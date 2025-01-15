#include "Lock.hpp"
#include "Thread.hpp"

#include <tuple>

#include <unistd.h>

using namespace wxy;

int cntTickets = 10000;
const int cntThread = 5;

void* GetTickets(Mutex mutex, pthread_t tid, unsigned int index)
{
    while(1)
    {
        {
			LockGuard lockGd(mutex);//每次循环结束，自动解锁
	        if(cntTickets > 0)
	        {
	            /*usleep() 的参数单位是微秒*/
	            usleep(989);//假如进来后先要检测一下
	            std::cout<< "Thread-" << index << " | Remaining tickets: " << --cntTickets << std::endl;	
	        }
	        else { break; }
		}//这对 {} 的作用就是避免 LockGuard在 usleep(666) 之后解锁, 
        usleep(666);
    }
    return nullptr;
}

int main()
{
    // pthread_t tid;
    // std::vector<pthread_t> tids;
    // Mutex mtx;
    
   

}
	
