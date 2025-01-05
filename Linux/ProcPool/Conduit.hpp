#pragma once

#include "Global.h"

class Conduit
{
public:
	Conduit(int fdWR, int SubID) : _fdWR(fdWR), _subID(SubID)
	{
		snprintf(_name, sizeof(_name) - 1, "SubProcID: %d, Writefd: %d\n", SubID, _fdWR);
	}

	int GetfdWR()const {return _fdWR;}
	const char* GetConduitName()const {return _name;}
	pid_t GetSubID()const {return _subID;}
private:
	int _fdWR; // 管道写端文件描述符
	pid_t _subID; // 对应的子进程的 PID
	char _name[128]; // 管道名称, 子进程ID + 对应管道的写端文件描述符
};