#include "Sort.h"
#include "Stack.h"

// 交换
void Swap(int *pLt, int *pRt)
{
	int temp = *pLt;
	*pLt = *pRt;
	*pRt = temp;
}

// 插入排序, 直接插入排序, O(n^2), 稳定
void InsertSort(int arr[], int szArr)
{
	int end;  // 数组下标, end 及 end 之前的数据已经是有序状态
	int temp; // 将temp插入到 [0, end]之中
	for (int i = 0; i < szArr - 1; ++i)
	{
		end = i;
		temp = arr[end + 1];
		for (; end >= 0; --end)
		{
			if (arr[end] > temp)
				arr[end + 1] = arr[end];
			else
				break;
		}

		/* for 循环结束到这里, 那可以说明
		 * 如果循环完全结束, 到达这里, 此时 end == -1, 把temp 放在 arr[0]即可, 即 arr[end + 1]
		 * 如果是通过 break 跳出的
		 * ** 要么刚进入循环就跳出来, 此时 直接把temp 放在 arr[end + 1], 其实相当于什么也没做,temp 本来就是 arr[end + 1]
		 * ** 要么是循环了若干次跳出来, 此时的情况是 arr[end] <= temp < arr[end + 1] == arr[end + 1 + 1], 显然将 temp 放在  arr[end + 1] 即可
		 */
		arr[end + 1] = temp;
	}
}

// 插入排序, 希尔排序
// 根据 gap 分组后,先把第一组完全排完, 再排下一组
void ShellSort1(int arr[], int szArr)
{
	int gap = szArr; // 初始步距
	int end;		 // 有序数组的尾标
	int temp;		 // 即将插入有序数组的新数据, 临时保存在这里
	while (gap > 1)
	{
		// 避免gap == 0
		// 当 gap == 2 进来后, 这里计算将得到 gap == 1, 此时的排序即是 InsertSort
		gap = gap / 3 + 1; 
		for (int i = 0; i < gap; ++i)
		{
			for (int j = i; j < szArr - gap; j += gap) // end + gap < szArr; 而 end 初始值等于j
			{
				end = j;
				temp = arr[end + gap];
				for (; end >= i; end -= gap)
				{
					if (arr[end] > temp)
					{
						arr[end + gap] = arr[end]; // 大的放后面
					}
					else
						break;
				}
				arr[end + gap] = temp; // 与直接插入排序逻辑相同
			}
		}
	}
}

// 插入排序, 希尔排序
// 根据 gap 分组后, 多组排序同时进行, 顺序基本是排完第一段 gap 再排下一段 gap
void ShellSort2(int arr[], int szArr)
{
	int gap = szArr;
	int end;
	int temp;
	while (gap > 1)
	{
		gap = gap / 3 + 1;
		for (int i = 0; i < szArr - gap; ++i) // i + gap < szArr, 而 end == i, 所以 end + gap < szArr
		{
			end = i;
			temp = arr[end + gap];
			for (; end >= 0; end -= gap)
			{
				if (arr[end] > temp)
				{
					arr[end + gap] = arr[end]; // 大的放后面
				}
				else
					break;
			}
			arr[end + gap] = temp;
		}
	}
}

// 选择排序, O(n^2), 不稳定
void SelectSort(int arr[], int szArr)
{
	int begin = 0;		 // 初始 起点下标
	int end = szArr - 1; // 初始 终点下标
	int iMin, iMax;		 // 最小值 最大值下标
	for (; begin < end; ++begin, --end)
	{
		iMin = begin, iMax = begin;
		for (int cur = begin + 1; cur <= end; ++cur)
		{
			if (arr[cur] < arr[iMin])
				iMin = cur;
			if (arr[cur] > arr[iMax])
				iMax = cur;
		}
		// 到这里, iMin 为最小值的下标,iMax 为最大值的下标

		Swap(&arr[begin], &arr[iMin]); // 只是交换了内容, iMin 此时已经不是最小值的下标了

		// 如果此时iMin的位置为最大值
		//  ** 如果 begin == iMax, 即上一步Swap之前, 最大值恰好在begin的位置
		//  ** 这将导致Swap之后,最大值被交换到iMin的位置
		//  ** 其他详见笔记
		if (iMax == begin)
			Swap(&arr[iMin], &arr[end]);
		else
			Swap(&arr[iMax], &arr[end]);
	}
}

int MedianOfThree(int arr[], int begin, int end)
{
	int median = (begin + end) / 2;
	if(arr[begin] > arr[end])
	{
		if(arr[median] > arr[begin]) 
			return begin;
		else if(arr[median] < arr[end]) 
			return end;
		else 
			return median;
	}
	else //arr[begin] <= arr[end]
	{
		if(arr[median] < arr[begin]) 
			return begin;
		else if(arr[median] > arr[end]) 
			return end;
		else 
			return median;
	}
}

// HeapSort, 也是选择排序的一种, 见 Heap 实现部分, 这里不再重复

// 交换排序(快排), Hoare 法
int __QuickSortHoare(int arr[], int begin, int end)
{
	int keyIndex = MedianOfThree(arr, begin, end);// 确保  arr[keyIndex] 非最大最小值
	Swap(&arr[keyIndex], &arr[end]);
	keyIndex = end;
	int pivot = arr[end];

	while(begin < end)
	{
		// 两个while 至少有一个得加上 = 的边界条件, 即<= 或 >=, 否则可能遇到  arr[begin] == arr[end] == pivot, 指针将卡住无法移动,导致死循环
		while(begin < end && arr[begin] <= pivot) ++begin;
		while(begin < end && arr[end] >= pivot) --end;
		if(begin < end) Swap(&arr[begin], &arr[end]);
	}
	Swap(&arr[begin], &arr[keyIndex]);
	return begin;
}

// 交换排序(快排), 挖坑法
int __QuickSortPit(int arr[], int begin, int end)
{
	int keyIndex = MedianOfThree(arr, begin, end);// 确保  arr[keyIndex] 非最大最小值
	Swap(&arr[keyIndex], &arr[end]);
	int pivot = arr[end]; // end 为坑

	while(begin < end)
	{
		// 两个while 至少有一个得加上 = 的边界条件, 即<= 或 >=, 否则可能遇到  arr[begin] == arr[end] == pivot, 指针将卡住无法移动,导致死循环
		while(begin < end && arr[begin] <= pivot) ++begin;
		arr[end]  = arr[begin]; // begin 将成为坑

		while(begin < end && arr[end] >= pivot) --end;
		arr[begin]  = arr[end]; // end 将成为坑
	}
	arr[begin] = pivot;
	return begin;
}

// 交换排序(快排), 双指针法
int __QuickSortDoublePtr(int arr[], int begin, int end)
{
	int keyIndex = MedianOfThree(arr, begin, end);// 确保  arr[keyIndex] 非最大最小值
	Swap(&arr[keyIndex], &arr[begin]); 
	int pivot = arr[begin]; // 双指针法适合以 begin 作为 基准值
	int prev = begin;
	int cur = begin + 1;

	while(cur <= end)
	{
		if(arr[cur] <= pivot)
		{
			++prev; // 这样prev 一定指向比 pivot 大的值
			Swap(&arr[prev], &arr[cur]); // prev 的位置一定 <= pivot
		}
		++cur;
	}
	Swap(&arr[prev], &arr[begin]);
	return prev;
}

// 快排, 递归方式, 最好O(nlog n), 最坏 O(n^2)
void RecursiveQuickSort(int arr[], int begin, int end)
{
	if(begin == end)
		return;
	//int div = __QuickSortHoare(arr, begin, end);// 霍尔法
	//int div = __QuickSortPit(arr, begin, end); // 挖坑法
	int div = __QuickSortDoublePtr(arr, begin, end);// 双指针法
	if(end - begin + 1 > 10)// 当超过某个数后, 再使用快速排序, 降低栈溢出的风险, 10 仅是示意, 可以是更合适的值
	{
		if (div > begin)
			RecursiveQuickSort(arr, begin, div - 1);
		if (div < end)
			RecursiveQuickSort(arr, div + 1, end);
	}
	else // 少于 10 个数, 使用插入排序
		InsertSort(arr, end - begin + 1);
}

void NonRecursiveQuickSort(int arr[], int begin, int end)
{
	struct Stack stak;
	StackInitial(&stak); // 初始化
	StackPush(&stak, end);
	StackPush(&stak, begin); // 这样可以先把顶部的数据取出来
	while(!StackEmpty(&stak))
	{
		int left = StackTop(&stak);
		StackPop(&stak);
		int right =  StackTop(&stak);
		StackPop(&stak);

		int div = __QuickSortDoublePtr(arr, left, right);// 这里仅用双指针法测试
		
		if(right - left + 1 > 10)// 当超过某个数后, 再使用快速排序, 降低栈溢出的风险, 10 仅是示意, 可以是更合适的值
		{
			if (div > begin)
			{
				StackPush(&stak, div - 1);
				StackPush(&stak, left);
			}
			if (div < end)
			{
				StackPush(&stak, right);
				StackPush(&stak, div + 1);
			}
		}
		else // 少于 10 个数, 使用插入排序
			InsertSort(arr, end - begin + 1);
	}
	StackDestroy(&stak);
}

// 冒泡排序也是交换排序, 这里不再实现