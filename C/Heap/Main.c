#include "Heap.h"
#include <stdio.h>

void Print(int arr[], int szArr)
{
	for (int i = 0; i < szArr; ++i)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main()
{
	int arr[] = {12, 25, -9, -85, -75, 95, 32, 58, 123, 42, 453, 12, 0, -2, -9, -5, -2, 2, 876, 1, 4};
	// int arr[] = {12, 25, 95, 32, 58, 123,  2, 1, 4};
	int sz = sizeof(arr) / sizeof(arr[0]);

	Heap *pHeap = malloc(sizeof(Heap));

	printf("Not Heap: ");
	Print(arr, sz);

	InitHeap(pHeap, arr, sz);
	printf("Heap Before Sorting: ");
	Print(pHeap->_pData, sz);

	// printf("Heap After Sorted: ");
	// HeapSort(pHeap);
	// Print(pHeap->_pData, sz);

	printf("PushBack 119: ");
	PushBack(pHeap, 119);
	Print(pHeap->_pData, sz);

	printf("Pop 6: "); // 删除下标为6的元素
	Pop(pHeap, 6);
	Print(pHeap->_pData, sz);

	return 0;
}