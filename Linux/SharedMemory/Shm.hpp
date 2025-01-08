// 目前仅简单实现, 还未加入 信号量 机制
#pragma once

#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <optional>
#include <iostream>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

const char pathName[64] = "."; // 路径名, 而不是路径下的文件名, 这里仅简单示意选择系统路径
const int projID = 'D'; // 68
const int szShm = 4096; // 4 的倍数, 如果4097 ,将分配 4096 * 2, 但实际可用的将只有4097

class SharedMemory
{
public:
	SharedMemory(const SharedMemory& x) = delete; // 禁止拷贝
	SharedMemory& operator=(const SharedMemory& x) = delete; // 禁止赋值

public:
	SharedMemory() : _shmID(-1), _shmAddr(nullptr),  _isOwner(false)
	{
		ConvertKey();
		std::cout << _key << std::endl;
	}
	void CreateShm() 
	{
		_shmID = __GetShm(IPC_CREAT | IPC_EXCL | 0600);// 需要有写权限
		 _isOwner = true;
		std::cout << _shmID << std::endl;
	} 
	void FindShm() 
	{
		_shmID =  __GetShm(IPC_CREAT);
		 _isOwner = false;
		std::cout << _shmID << std::endl;
	}
	
	void AttachShm()
	{
		void* ret =  shmat(_shmID, NULL, 0);
		if(ret == (void*)-1)
		{
			perror("Attach shared memory failed");
			exit(1);
		}
		_shmAddr = ret;
		std::cout << _shmAddr << std::endl;
	}

	void DetachShm()
	{
		int ret =  shmdt(_shmAddr);
		if(ret == -1)
		{
			perror("Detach shared memory failed");
			exit(1);
		}
	}

	~SharedMemory()
	{
		if( _isOwner)
		{
			int ret = shmctl(_shmID, IPC_RMID, NULL);
			if (ret == -1)
			{
				perror("Destroy shared memory failed");
				exit(1);
			}
			std::cout << "Destroy successed" << std::endl;
		}
	}

	void* Addr() {return _shmAddr;}
	int Size() {return szShm;}

private:
	void ConvertKey()
	{
		key_t ret = ftok(pathName, projID);
		if(ret == -1)
		{
			throw std::runtime_error("Convert key failed");
		}
		_key = ret;
	}

	int __GetShm(int shmFlag)
	{
		int ret = shmget(_key, szShm, shmFlag);
		if(ret == -1)
		{
			if (errno == EEXIST) 
			{
				fprintf(stderr, "the segment already exists");
				exit(1);
			}
			else
			{
				perror("Allocate shared memory failed");
				exit(1);
			}
		}
		return ret;
	}
private:
	key_t _key;
	int _shmID;
	void* _shmAddr;
	bool _isOwner; // 标志是否允许销毁 共享内存
};