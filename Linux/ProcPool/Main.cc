#include "ProcPool.hpp"
#include "Tasks.hpp"

int main()
{
	std::vector<std::function<void(void)>> tasks;
	LoadTasks(tasks);

	ProcPool procPool(tasks);
	procPool.CreateSubProcs();
	procPool.RandomDistribution(20);
}