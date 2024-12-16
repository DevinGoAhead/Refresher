#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

const size_t numCMD = 64; // 指令参数数量的最大值
const size_t numCMDStr = 1024; // 指令参数所包含字符的最大值

int main()
{
	int exitSignal; // 保存上一次命令的退出信号
	int exitCode; // 保存上一次命令的退出码
	
	while(1)// 循环执行指令
	{
		printf("UserName@MyShell: "); //指令前的用户提示
		fflush(stdout); // 刷新标准输出流缓冲
		
		// 1. 获取指令,存入字符串中
		char strCMD[numCMDStr]; // 缓冲区, 存放从标准输入中获取的指令
		fgets(strCMD, numCMDStr - 1, stdin); // 获取指令并放入 strCMD 中, 留一个位置给 '0'
		
		size_t lenStrCMD = strlen(strCMD); // 实际输入的指令包含的字符的个数
		strCMD[lenStrCMD - 1] = '\0'; // 执行指令时, 总要使用 Enter 键, 这里消除该字符, 否则后面解析为指令后将时非法指令

		// 测试
		// printf("%s", strCMD);

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