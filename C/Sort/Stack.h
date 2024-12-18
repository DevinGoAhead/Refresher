#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef int SKDataType;

#define  capacity_initial 5

typedef struct Stack
{
	SKDataType* _p;
	int top;
	int capacity;
}SK;

//声明
void StackInitial(SK* ps);//栈初始化
void StackPush(SK* ps, SKDataType data);//入栈
void StackPop(SK* ps);//出栈
int StackTop(SK* ps);//获取栈顶位置
int StackEmpty(SK* ps);//判断栈是否为空
void StackDestroy(SK* ps);//销毁