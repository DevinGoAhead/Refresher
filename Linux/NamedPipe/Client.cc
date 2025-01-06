#include "Global.h"


int main()
{
	int fd = open(pathName, O_WRONLY); // 不需要 O_CREATE, 自然也不需要mode, 因为文件必定是已经存在的
	if(fd == -1)
	{
		perror("open failed");
		exit(1);
	}
	char buffer[1024];
	while(true)
	{
		std::cout << "Please input: ";
		char* ret = fgets(buffer, sizeof(buffer) - 1, stdin); // 留一个位置给 '\0', 备用
		if(strlen(buffer) == 1)
		{
			std::cout << "Empty Input, try again" << std::endl;
			continue;
		}
		if(ret == NULL)
		{
			perror("ERROR INPUT");
			continue;
		}
		
		buffer[strlen(buffer) - 1] = '\0'; // 最后一个字符是回车键对应的 \n, 是无意义的, 将其替换为 '\0'
		int size = write(fd, buffer, strlen(buffer));
		if(size == -1)
		{
			perror(" write failed");
			break;
		}
	}
	return 0;
}