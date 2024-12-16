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
		myArgv[i] = token; // 最后一个指针需要为 NULL, 作为结束标记

		// 3. 调用替换程序, 执行解析的指令
		pid_t id = fork();

		if(id == 0) // 子进程执行替换程序
		{
			execvp(myArgv[0], myArgv);// 注意, 只有调用程序失败, 下面两行代码才会生效
			perror("execvp failed"); // 打印错误信息
			exit(10);// 执行完成后, 子进程退出
		}

		// 父进程, 这里仅负责回收子进程
		printf("%d\n", id); // 测试
		int status;
		waitpid(id, &status, 0);
		printf("exit success: exit signal(%d), exit code(%d)", status & 0x007F, (status >> 8) & 0x00FF);
		printf("\n");
	}
}