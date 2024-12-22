#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

enum Redirec
{
	NoneRedirect, InputRedirec, OutputRedirec, AppendRedirec
};

enum Redirec redirec = NoneRedirect; // 默认无重定向

const size_t numCMD = 64; // 指令参数数量的最大值
const size_t numCMDStr = 1024; // 指令参数所包含字符的最大值
char fNameRedirec[128]; // 存储重定向文件的文件名(路径 + 文件名)

#define TrimSpace(str)/*消除空格*/ \
	{\
		while(isspace(*str) != 0)/*直到 str 指向的字符不为空格*/\
			++str;\
	}

void HasRedirec(char str[]) // 检查指令中是否包含重定向
{
	while(*str !=  '\0')
	{
		if(*str == '>')//输出重定向
		{
			*str = '\0'; // 这样, 在使用 strtok 解析字符串为指令时, 将不会解析重定向符号及后面的内容
			++str; // 此时str 可能指向空格, 也可能 >
			if(*str == '>')// 追加重定向
			{
				redirec = AppendRedirec;
				//*str = '\0'; // 这一步不需要
				++str; // 此时str 指向空格
			}
			else
			{
				redirec = OutputRedirec;
			}
			// 查找文件名
			TrimSpace(str); // 去掉 定向符号 与 文件名之间的空格
			//printf(str);
			strcpy(fNameRedirec, str);
			//printf(fNameRedirec);
		}
		else if(*str == '<') // 输入重定向
		{
			redirec = InputRedirec;
			*str = '\0'; // 同上
			++str; // 此时str 指向空格
						// 查找文件名
			TrimSpace(str); // 去掉 定向符号 与 文件名之间的空格
			strcpy(fNameRedirec, str);
		}
		else // 非重定向, 继续向后查找
			++str;
	}
}

int main()
{
	int exitSignal; // 保存上一次命令的退出信号
	int exitCode; // 保存上一次命令的退出码
	
	while(1)// 循环执行指令
	{
		printf("UserName@MyShell: "); //指令前的用户提示
		fflush(stdout); // 刷新标准输出流缓冲
		
		redirec = NoneRedirect; // 重置重定向标识符

		// 1. 获取指令,存入字符串中
		char strCMD[numCMDStr]; // 缓冲区, 存放从标准输入中获取的指令
		fgets(strCMD, numCMDStr - 1, stdin); // 获取指令并放入 strCMD 中, 留一个位置给 '0'
		
		size_t lenStrCMD = strlen(strCMD); // 实际输入的指令包含的字符的个数
		strCMD[lenStrCMD - 1] = '\0'; // 执行指令时, 总要使用 Enter 键, 这里消除该字符, 否则后面解析为指令后将时非法指令

		// 测试
		// printf("%s", strCMD);

		// 如果包含重定向符号, 做一些处理
		HasRedirec(strCMD);
		
		// 2. 将字符串解析为指令
		char* myArgv[numCMD];
		size_t i = 0;
		char* token;
		for(token = strtok(strCMD, " "); token != NULL; token =  strtok(NULL, " ")) //输入指令时, 空格 即是分隔符
		{
			myArgv[i++] = token;
		}

		// 设置 ls 自动开启语法高亮
		if(strcmp(myArgv[0], "ls") == 0 && i < (numCMD - 1))// 不能超过 numCMD 个命令
		{
			myArgv[i++] = "--color=auto";
		}

		myArgv[i] = token; // 最后一个指针需要为 NULL, 作为结束标记

		// 修正 内建命令 cd
		if(myArgv[0] != NULL && strcmp(myArgv[0], "cd") == 0 && myArgv[1] != NULL)
		{
			chdir(myArgv[1]); // 更改cwd
			continue;// 不需要子进程调用程序执行了
		}

		// 修正 内建命令 echo
		if(myArgv[0] != NULL && strcmp(myArgv[0], "echo") == 0)
		{
			if(myArgv[1] != NULL && strcmp(myArgv[1], "$?") == 0)
			{
				printf("%d, %d\n", exitSignal,  exitCode);
			}
			else // 普通变量
			{
				printf("%s\n", myArgv[1]);
			}
		}

		// 3. 调用替换程序, 执行解析的指令
		pid_t id = fork();

		if(id == 0) // 子进程执行替换程序
		{
			// 因为命令是在子进程执行的, 而进程文件描述符表是进程间独立的, 因而需要在子进程中执行重定向操作
			switch(redirec)
			{
				case NoneRedirect:
				{
					break;
				}
				case InputRedirec:
				{
					int fd = open(fNameRedirec, O_RDONLY);
					if(fd == 0)
					{
						perror("Failed to open file: ");
						exit(errno);
					}
					dup2(fd, 0);// 0: stdin
					break;
				}
				case OutputRedirec:
				case AppendRedirec://两种情况合并处理
				{
					umask(0); // 将掩码置为 0000
					if(redirec == OutputRedirec)
					{
						int fd = open(fNameRedirec, O_WRONLY | O_CREAT | O_TRUNC, 0666); // 只写, 创建, 清空
						if(fd == 0)
						{
							perror("Failed to open file: ");
							exit(errno);
						}
						dup2(fd, 1);//1: stdout
						break;
					}
					else // (redirec == AppendRedirec)
					{
						int fd = open(fNameRedirec, O_WRONLY | O_CREAT | O_APPEND, 0666); // 只写, 创建, 追加
						if(fd == 0)
						{
							perror("Failed to open file: ");
							exit(errno);
						}
						dup2(fd, 1);//1: stdout
						break;
					}
				}
			}
			execvp(myArgv[0], myArgv);// 注意, 只有调用程序失败, 下面两行代码才会生效
			perror("execvp failed"); // 打印错误信息
			exit(10);// 执行完成后, 子进程退出
		}

		// 父进程, 这里仅负责回收子进程
		//printf("%d\n", id); // 测试
		int status;
		waitpid(id, &status, 0);
		
		exitSignal =  status & 0x007F;
		exitCode = (status >> 8) & 0x00FF;

		printf("exit success: exit signal(%d), exit code(%d)",exitSignal, exitCode);
		printf("\n");
	}
}