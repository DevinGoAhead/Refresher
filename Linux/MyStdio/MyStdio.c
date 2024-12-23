#include "MyStdio.h"

FILE_ * fopen_(const char* pathName, char* mode)
{
	int flags = 0;
	// 确定flags
	// 这里的 mode 指的是C 库中fopen 函数的mode, 而不是系统库中open 函数的 mode
	if(!strcmp("r", mode)) {flags |= O_RDONLY;}
	else if(!strcmp("w", mode)) {flags |= (O_WRONLY | O_CREAT | O_TRUNC);}
	else if(!strcmp("a", mode)) {flags |= (O_WRONLY | O_CREAT | O_APPEND);}
	else {/*...其它 mode 判断*/}

	// 确定mode(权限), 系统库中 open 函数的 mode
	int fd;
	if (flags & O_RDONLY)// 只读, 不需要权限
	{
		fd = open(pathName, flags);
	} 
	else // 写 或 追加
	{
		umask((mode_t)0);
		fd = open(pathName, flags, 0666);
	}

	// 如果 open 失败
	if(fd == 0) 
	{
		char* errMsg = strerror(errno);
		write(2, errMsg, sizeof(errMsg));
		return NULL;
	}
	
	// 到这里, 表示打开文件成功了
	FILE_* stream = (FILE_*)malloc(sizeof(FILE_));
	if(stream == NULL) // 申请堆空间失败
	{
		char* errMsg = strerror(errno);
		write(2, errMsg, sizeof(errMsg));
		return NULL;
	}

	// 到这里, 表示打开文件, 申请堆空间都成功了, 可以初始化 FILE_
	stream->_fileno = fd;
	stream->_bufType = SyncLine; // 严格而言, 这里应该根据文件类型判断缓冲类型, 本例仅示意原理, 因此直接判断位行缓冲
	memset((void*)stream->_buffer, 0, CAPACITY); // 全部置为0
	return stream;
}

int fclose_(FILE_ *stream)
{
	assert(stream);
	write(stream->_fileno, stream->_buffer, stream->_size); // 关闭文件前要刷新, 直接调用 fflush_ 也可以
	stream->_size = 0; // 惰性删除缓冲区中内容
	int ret = close(stream->_fileno);
	return ret == 0 ? ret : EOF;
}

int fflush_(FILE_ *stream)
{
	assert(stream);
	int ret = write(stream->_fileno, stream->_buffer, stream->_size); // 写入到文件(其实是内核的缓冲区)
	if(ret > 0)
	{
		stream->_size = 0; // 惰性删除缓冲区中内容
		return 0;
	}
	return EOF;
}

size_t fwrite_(const void* ptr, size_t count, FILE_* stream)
{
	assert(ptr && stream);
	memcpy(stream->_buffer, ptr, count); // 先把数据写入到stream(语言级别的缓冲区)
	stream->_size += count;

	// 根据情况判断是否要刷新到内核中的文件(实际是内核中的缓冲区)
	switch(stream->_bufType)
	{
		case SyncNow:
		{
			int ret = write(stream->_fileno, stream->_buffer, stream->_size); // 等效于调用 fflush_
			stream->_size = 0; // 惰性删除缓冲区中内容
			return ret;
		}
		case SyncLine:
		{
			size_t i = stream->_size;
			while(i)
			{
				if(stream->_buffer[--i] == '\n') // 判断是否包含换行符
				{
					int ret = write(stream->_fileno, stream->_buffer, i + 1); // 同上
					int newSize = stream->_size - i - 1;
					memmove(stream->_buffer, &stream->_buffer[i + 1], newSize);
					stream->_size = newSize;
					return ret;
				}
			}
			break;
		}
		case SyncFull:
		{
			if(stream->_size == CAPACITY)
			{
				int ret = write(stream->_fileno, stream->_buffer, stream->_size); // 同上
				stream->_size = 0; // 惰性删除缓冲区中内容
				return ret;
			}
			break;
		}
		// case other SyncType ...
		default:
			return 0; // 可以做更细节的处理
	}
	return 0;
}