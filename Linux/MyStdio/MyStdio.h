#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define CAPACITY 4096 // FILE 中能够存放的数据的最大字节数(字符数)

enum BufferType// 缓冲类型
{
	SyncNow, // 即时缓冲
	SyncLine, // 行缓冲
	SyncFull // 全缓冲
};

typedef struct FILE_
{
	char _buffer[CAPACITY]; // 语言级别的缓冲区, 严格而言, 这里应该是动态增长, 本例仅示意原理, 因此采用固定容量
	size_t _size; // 缓冲区当前存放的 字符 的数量
	int _fileno; // 关联 文件系统中的文件描述符 fd
	enum BufferType _bufType; //缓冲类型
}FILE_;

FILE_ * fopen_(const char* pathName, char* mode); // 这里的 mode 是 系统 open 函数 中的 flag + mode
int fclose_(FILE_ *stream);
int fflush_(FILE_ *stream); // 手动将数据从FILE(语言级别的缓冲区) 刷新到内核中的文件
size_t fwrite_(const void* ptr, size_t count, FILE_* stream); // 将 count 个字节的数据, 从 ptr 指向的内存写到 stream, 相对 C 库函数做了一些简化