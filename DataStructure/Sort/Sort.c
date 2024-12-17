#include "Sort.h"

// 交换
void Swap(int *pLt, int *pRt)
{
	int temp = *pLt;
	*pLt = *pRt;
	*pRt = temp;
}

// 插入排序, 升序
void InsertSort(int arr[], int szArr)
{
	int end;  // 数组下标, end 及 end 之前的数据已经是有序状态
	int temp; // 将temp插入到 0~end之中
	for (int i = 0; i < szArr - 1; ++i)
	{
		end = i;
		temp = arr[end + 1];
		for (; end >= 0; --end)
		{
			if (temp < arr[end])
				arr[end + 1] = arr[end];
			else
				break;
		}

		// for 循环结束到这里, 那可以说明temp 此时是最小值, 将其放在arr[0]即可, 注意, 如果是这种情况, 此时end == -1
		// 或 找到了 temp >= arr[end], 此时
		// ** 要么刚进入循环就跳出来, 此时 直接把temp 放在 arr[end + 1], 其实相当于什么也没做,temp本来就再arr[end + 1]
		// ** 要么是循环了若干次跳出来, 在上一个循环中, arr[end + 1] == arr[end + 1 + 1], 此时把temp放在 arr[end + 1] 即可
		arr[end + 1] = temp;
	}
}

// 希尔排序, 升序
// 根据 gap 分组后,先把第一组完全排完, 再排下一组
void ShellSort1(int arr[], int szArr)
{
	int gap = szArr; // 初始步距
	int end;		 // 有序数组的尾标
	int temp;		 // 即将插入有序数组的新数据, 临时保存在这里
	while (gap > 1)
	{
		gap = gap / 3 + 1; // 避免gap == 0
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

// 希尔排序, 升序
// 根据 gap 分组后, 多组排序同时进行, 顺序基本是排完第一段 gap 再排下一段 gap
void ShellSort2(int arr[], int szArr)
{
	int gap = szArr;
	int end;
	int temp;
	while (gap > 1)
	{
		gap = gap / 3 + 1;
		for (int i = 0; i < szArr - gap; ++i) // end + gap < szArr; 而 end 初始值等于i
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

// 选择排序, 升序
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