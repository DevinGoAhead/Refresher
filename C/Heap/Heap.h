// 本例实现小根堆
// 作为练习, 对诸如pHeap是否为NULL, pos是否越界等未做判断

#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef int Type;
#define DEFAULT_NEW_CAP 10 // 默认扩容数值

typedef struct Heap
{
	Type *_pData;
	int _size;
	int _capacity;
} Heap;

void Swap(Type *pLt, Type *pRt)
{
	Type temp = *pLt;
	*pLt = *pRt;
	*pRt = temp;
}

// 用于插入数据时检查是否需要扩容
void IsHeapAlloc(Heap *pHeap)
{
	int oldCap = pHeap->_capacity;
	int newCap;
	if (oldCap <= pHeap->_size)
	{
		newCap = oldCap + DEFAULT_NEW_CAP;
		Type *pTemp = (Type *)realloc((void *)pHeap->_pData, newCap * sizeof(Type));
		assert(pTemp);
		pHeap->_pData = pTemp;
		pHeap->_capacity = newCap;
	}
}

// 将原始数组中内容，拷贝到堆中
// 开辟内存，拷贝数据
void InitHeap(Heap *pHeap, Type arr[], int szArr)
{
	Type *pTemp = (Type *)calloc(szArr, sizeof(Type));
	assert(pTemp);
	pHeap->_pData = pTemp;
	pTemp = NULL;
	pHeap->_size = szArr;
	pHeap->_capacity = szArr;
	memcpy((void *)pHeap->_pData, (void *)arr, sizeof(Type) * szArr);

	// 从最后一个非叶节点，即最后一个节点的父节点开始，依次 向上 调用 向下调整算法
	for (int i = (szArr - 1 - 1) / 2; i >= 0; --i)
	{
		AdjustToDown(pHeap, i, pHeap->_size);
	}
}

// 假设 parent 节点以下的子树已经处于 堆标准的有序 状态，调用向下调整算法
// 如果使单纯的初始化堆，Heap *pHeap, int parent 就可以
// 为了配合堆排序，这里还需要引入一个numNode参数,numNode 是所有相关节点数量
void AdjustToDown(Heap *pHeap, int parent, int numNode)
{
	int minChild = 2 * parent + 1; // 假设左孩子为较小节点
	while (minChild < numNode)	   // 较小的子节点必须存在
	{
		if (minChild + 1 < numNode && /*假设右孩子存在，且右孩子小于左孩子*/
			pHeap->_pData[minChild + 1] < pHeap->_pData[minChild])
			++minChild; // 令右孩子为较小节点
		if (pHeap->_pData[parent] > pHeap->_pData[minChild])
		{
			Swap(&(pHeap->_pData[parent]), &(pHeap->_pData[minChild]));
			parent = minChild; // 将调整后的子节点，作为新的基点（即新的父节点），想继续向下调整
			minChild = 2 * parent + 1;
		}
		else // 否则就满足小堆了，直接跳出循环
			break;
	}
}

/*
 * 堆排序，本例我们创建的是小堆，可用于做升序排序的基础
 * 堆排序是交换排序的一种
 * 堆本身并不是有序的，但可以确定的是（就小堆而言）根节点一定是最小的
 * ** 基于该特点，排序的思路为
 * ** ** 将根节点与尾节点交换，此时尾节点将是最小值
 * ** ** 对排除尾节点后的堆 的根节点执行向下调整算法，使其恢复小堆状态
 * ** ** 循环上述两步，直至小堆仅剩一个节点
 * 排序完成后将不再满足堆结构要求
 */
void HeapSort(Heap *pHeap)
{
	int numNode = pHeap->_size;
	while (numNode > 1)
	{
		Swap(&(pHeap->_pData[0]), &(pHeap->_pData[--numNode])); // 尾节点将是最小值
		AdjustToDown(pHeap, 0, numNode);
	}
}

// 在堆尾插入数据
// 在根部插入会破坏堆结构，恢复堆很麻烦，因此这里选择在堆尾插入
void PushBack(Heap *pHeap, Type data)
{
	IsHeapAlloc(pHeap); // 检查是否需要扩容
	pHeap->_pData[pHeap->_size++] = data;

	AdjustToUp(pHeap, pHeap->_size - 1);
}

// 假设 child 节点以上已经处于 堆标准的有序 状态，调用向上调整算法
void AdjustToUp(Heap *pHeap, int child)
{
	int parent = (child - 1) / 2;
	while (child > 0)
	{

		if (pHeap->_pData[parent] > pHeap->_pData[child])
		{
			Swap(&(pHeap->_pData[parent]), &(pHeap->_pData[child])); // 若父节点大于子节点
			child = parent;
			parent = (child - 1) / 2;
		}
		else
			break;
	}
}

// 删除任意位置的数据
// 删除堆尾数据完全不会破坏堆结构，删除其他位置的数据会极大的破坏堆结构
//**基于此，这里采取先将要删除的数据交换到堆尾，然后再删除，这样仅仅影响pos所在子树的以上部分
//**删除后，再从pos位置开始恢复堆结构，代价远远小于直接删除pos
void Pop(Heap *pHeap, int pos)
{
	int sz = pHeap->_size;
	assert(pos < sz - 1);

	Swap(&(pHeap->_pData[sz - 1]), &(pHeap->_pData[pos]));
	--(pHeap->_size);

	// 恢复堆结构
	for (int i = pos; i > 0; --i)
		AdjustToDown(pHeap, i, pHeap->_size);
}

// 删除堆尾元素
void PopBack(Heap *pHeap)
{
	pHeap->_size--;
}

// 销毁堆
void HeapDestroy(Heap *pHeap)
{

	free(pHeap->_pData);
	pHeap->_size = 0;
	pHeap->_capacity = 0;
	// free(pHeap);
}
