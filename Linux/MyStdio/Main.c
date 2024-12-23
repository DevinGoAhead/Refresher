#include "MyStdio.h"

int main()
{
	FILE_* pf = fopen_("test.txt", "a");

	//char str[] = "Hello IO\n";//行缓冲
	char str[] = "Hello IO";//非行缓冲
	int cnt = 10;
	while(cnt)
	{
		fwrite_(str, strlen(str), pf);
		sleep(1);
		printf("cnt: %d\n",cnt--);  
	}
	fclose_(pf);
}