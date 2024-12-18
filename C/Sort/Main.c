#include "Sort.h"
#include <stdio.h>

void Print(int arr[], int szArr)
{
	for(int i = 0; i < szArr; ++i )
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void test1(int arr[], int szArr)//直接插入排序
{
	printf("InsertSort: ");
	InsertSort(arr, szArr);
	Print(arr, szArr);
}

void test2(int arr[], int szArr)//希尔排序1
{
	printf("ShellSort1: ");
	ShellSort1(arr, szArr);
	Print(arr, szArr);
}

void test3(int arr[], int szArr)//希尔排序2
{
	printf("ShellSort2: ");
	ShellSort2(arr, szArr);
	Print(arr, szArr);
}

void test4(int arr[], int szArr)//选择排序
{
	printf("SelectSort: ");
	SelectSort(arr, szArr);
	Print(arr, szArr);
}

void test5(int arr[], int szArr)//交换排序
{
	printf("RecursiveQuickSort: ");
	//RecursiveQuickSort(arr, 0, szArr - 1);
	NonRecursiveQuickSort(arr, 0, szArr - 1);
	Print(arr, szArr);
}

// void test5(int arr[], int szArr)//交换排序
// {
// 	printf("SelectSort: ");
// 	SelectSort(arr, szArr);
// 	Print(arr, szArr);
// }

int main()
{
	int arr[] = {223, 29, 36, 112, -8, 184, 42, 59, -24, 150, 99, 202, -8, 30, 222, 0, 45};
	int sz = sizeof(arr) / sizeof(arr[0]);
	
	printf("Before Sorting: ");
	Print(arr, sz);
	
	//test1(arr, sz);//直接插入发排序
	//test2(arr, sz);//希尔排序1
	//test3(arr, sz);//希尔排序2
	//test4(arr, sz);//选择排序
	test5(arr, sz);//快速排序-递归

}