#include "BSTree.hpp"

#include <iostream>

using namespace wxy;

int main()
{
	BSTree<int> bsTree;
	bsTree.Insert(10);
	bsTree.Insert(12);
	bsTree.Insert(8);
	bsTree.Insert(14);
	bsTree.Insert(6);
	bsTree.Insert(16);
	bsTree.Insert(9);
	bsTree.Insert(7);
	bsTree.Insert(11);
	bsTree.Insert(5);
	bsTree.Insert(13);
	bsTree.Insert(3);
	bsTree.Insert(17);

	bsTree.PrintInOrder();
	bsTree.PrintLevelOrder();
	std::cout << (bsTree.Find(7))->_key << std::endl;
	// bsTree.Erase(9);// 左右子节点皆为空
	// bsTree.Erase(5);// 右子节点为空
	// bsTree.Erase(16); // 左右子节点皆不为空, 且左子节点的左右子节点皆为空
	//bsTree.Erase(6);// 左右子节点皆不为空, 且左子节点的左子节点为空
	// bsTree.Erase(8);// 左右子节点皆不为空, 且左子节点的左子节点为空
	bsTree.Erase(10);// 根节点, 右左节点皆不为空
	bsTree.PrintLevelOrder();
}