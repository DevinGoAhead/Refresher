#pragma once
#include "Global.h"

// 简单创建一些任务, 仅示意

void IOTask() {std::cout << "IOTask" << std::endl;}
void TraceTask() {std::cout << "TraceTask" << std::endl;}
void DownLoadTask() {std::cout << "DownLoadTask" << std::endl;}
void ShadeTask() {std::cout << "ShadeTask" << std::endl;}

void LoadTasks(std::vector<std::function<void(void)>>& tasks)
{
	tasks.emplace_back(IOTask);
	tasks.emplace_back(TraceTask);
	tasks.emplace_back(DownLoadTask);
	tasks.emplace_back(ShadeTask);
}