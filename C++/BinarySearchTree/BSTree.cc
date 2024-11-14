#include "BSTree.hpp"

#include <iostream>
#include <string>

using namespace wxy;

int main()
{
	BSTree<int, std::string> bsTree;
	bsTree.Insert(10, "十");
	bsTree.Insert(12, "十二");
	bsTree.Insert(8, "八");
	bsTree.Insert(14, "十四");
	bsTree.Insert(6, "六");
	bsTree.Insert(16, "十六");
	bsTree.Insert(9, "九");
	bsTree.Insert(7, "七");
	bsTree.Insert(11, "十一");
	bsTree.Insert(5, "五");
	bsTree.Insert(13, "十三");
	bsTree.Insert(3, "三");
	bsTree.Insert(17, "十七");

	bsTree.PrintInOrder();
	bsTree.PrintLevelOrder();
	//std::cout << (bsTree.Find(7))->_key << std::endl;
	// bsTree.Erase(9);// 左右子节点皆为空
	// bsTree.Erase(5);// 右子节点为空
	// bsTree.Erase(16); // 左右子节点皆不为空, 且左子节点的左右子节点皆为空
	//bsTree.Erase(6);// 左右子节点皆不为空, 且左子节点的左子节点为空
	// bsTree.Erase(8);// 左右子节点皆不为空, 且左子节点的左子节点为空
	bsTree.Erase(10);// 根节点, 右左节点皆不为空
	bsTree.PrintLevelOrder();
}