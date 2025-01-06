#include "Global.h"

#include <cstdio>
#include <cstdlib>


#include <sys/types.h>
#include <sys/stat.h>


int main()
{
	umask(0); // 设置掩码
	
	// 创建管道文件
	int ret = mkfifo(pathName, 0660); 
	if(ret == -1)
	{
		perror("mkfifo failed");
		exit(1);
	}

	// 以只读模式打开文件
	int fd = open(pathName, O_RDONLY); 
	if(fd == -1)
	{
		perror("open failed");
		exit(1);
	}

	//从管道文件读取
	while(true)
	{
		char buffer[1024];
		int size = read(fd, buffer, sizeof(buffer) - 1);
		if(size == -1)
		{
			perror("Read failed");
			break;
		}
		else if(size == 0)
		{
			perror("None to be read. Client has quit, me too");
			break;
		}
		else
		{

			buffer[size] = '\0'; // 添加一个 \0, 便于c c++ 函数使用
			if(strlen(buffer) == 0) {continue;} // 如果空消息

			std::cout << "Server Read Message: " << buffer << std::endl;
		}
	}

	unlink(pathName);// 关闭(删除)管道文件
	return 0;
}