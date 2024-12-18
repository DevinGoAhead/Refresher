#include "Stack.h"

//初始化
void StackInitial(SK * ps)
{
	////思路一：初始化时即开辟内存
	//ps->_p=malloc(capacity_initial * sizeof(SKDataType));
	//memset((ps, capacity_initial * sizeof(SKDataType),0));//初始化为0
	//ps->top = 0;
	//ps->capacity = capacity_initial;
	
	//思路二：初始化时不开辟内存，待插入时再插入
	ps->_p = NULL;
	ps->top = ps->capacity = 0;
}

//入栈-其实是尾插，这里将右侧作为栈顶侧
void StackPush(SK* ps,SKDataType data)
{
	/*assert(ps);*/
	if (ps->top >= ps->capacity)
	{
		ps->capacity = ps->capacity==0 ? capacity_initial : ps->capacity * 2;//判断需要开辟空间的大小，若0，则开辟默认内存大小，若非0，则当前容量二倍
		SKDataType *p_temp = realloc(ps->_p,ps->capacity * sizeof(SKDataType));
		if (p_temp == NULL)
			perror("内存开辟失败");
		else
		{
			ps->_p = p_temp;
			memset(ps->_p + ps->top, 0, (ps->capacity - ps->top) * sizeof(SKDataType));//将新开辟的空间初始化为0。该步可选,非必须
		}
	}
	ps->_p[ps->top] = data;
	ps->top++;
}

//出栈-其实是尾删
void StackPop(SK* ps)
{
	assert(ps);
	assert(ps->top);
	ps->top--;
}

//获取栈顶位置/数据
int StackTop(SK* ps)
{
	assert(ps);
	return ps->top;
	//return ps->_p[ps->top - 1];//获取栈顶数据
}

//判断栈是否为空，0表示空，1表示非空
int StackEmpty(SK* ps)
{
	int isEmpty = ps->top == 0 ? 1 : 0;
	return isEmpty;
}

//销毁栈
void StackDestroy(SK* ps)
{
	ps->capacity = 0;
	free(ps->_p);
	ps->_p = NULL;
}