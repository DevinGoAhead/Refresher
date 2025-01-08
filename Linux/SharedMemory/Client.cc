#include "Shm.hpp"

int main()
{
	SharedMemory shm;
	shm.FindShm();
	shm.AttachShm();
	int cnt = 20;
	char* addr = static_cast<char*>(shm.Addr());
	while(cnt--)
	{
		snprintf(addr, shm.Size() - 1, "Client process PID: %d, sending %d", getpid(), cnt); // 直接写入共享内存
		sleep(1);
	}
	shm.DetachShm();
	return 0;
}