#include "Shm.hpp"

#include <cstdio>
#include <cstdlib>


#include <sys/types.h>
#include <sys/stat.h>


int main()
{
	SharedMemory shm;
	shm.CreateShm();
	shm.AttachShm();
	char* addr = static_cast<char*>(shm.Addr());
	int cnt = 30;
	sleep(5);
	while(cnt--)
	{
		printf("Server read message: %s\n", addr);
		sleep(1);
	}
	shm.DetachShm();
	return 0;
}