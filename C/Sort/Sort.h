void Swap(int* pLt, int *pRt); //交换
int MedianOfThree(int arr[], int begin, int end); // 三数取中
int __QuickSortHoare(int arr[], int begin, int end);//Hoare 法
int __QuickSortPit(int arr[], int begin, int end);//挖坑法
int __QuickSortDoublePtr(int arr[], int begin, int end); // 双指针法

//以下皆按升序排序
void InsertSort(int arr[], int szArr); // 插入排序
void ShellSort1(int arr[], int szArr); // 希尔排序
void ShellSort2(int arr[], int szArr); // 希尔排序
void SelectSort(int arr[], int szArr); // 选择排序
void RecursiveQuickSort(int arr[], int begin, int end); // 交换排序, 递归
void NonRecursiveQuickSort(int arr[], int begin, int end); // 交换排序,非递归方式