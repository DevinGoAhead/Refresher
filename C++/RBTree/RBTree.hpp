//仅实现了 insert 函数

#include <iostream>
#include <queue>
#include <utility>
#include <cassert>
#include <cstdlib>

namespace wxy
{
	enum Color
	{
		red = 1,
		black = 2
	};
	
	template <typename K, typename V>
	struct __RBTreeNode
	{
	public:
		__RBTreeNode(const std::pair<K, V>& kv)
			: _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr),_color(red)// 默认红色,代价更小
		{
		}

	public:
		std::pair<K, V> _kv;
		__RBTreeNode<K, V> *_left;
		__RBTreeNode<K, V> *_right;
		__RBTreeNode<K, V> *_parent;
		Color _color;//red or black
	};

	template <typename K, typename V>
	class RBTree
	{
	public:
		using Node = __RBTreeNode<K, V>;

	public:
		RBTree()
			: _root(nullptr)
		{
		}
		bool Insert(const std::pair<K, V>& kv);
		void PrintInOrder()const;
		void PrintLevelOrder()const;
		void TreeHeight(); // 输出 树的最大高度和最小高度
		bool isValidRBTree(); // 检查二叉树 是否符合RBTree的 4 个条件
	private:
		void __PrintInOrder(Node* node)const;
		void RtRotate(Node* node); // 右旋, node 可能是parent ,也可能是 gparent
		void LtRotate(Node* node); // 左旋, node 可能是parent ,也可能是 gparent
		int __TreeMaxHeight(Node* node);
		int __TreeMinHeight(Node* node);
		bool __isValidRBTree(Node* node, int basicBlackNum, int anyPathBlackNum);
	private:
		Node *_root;
	};

	template <typename K, typename V>
	bool RBTree<K, V>::Insert(const std::pair<K, V>& kv)
	{
		if (_root == nullptr) // 空树
		{
			_root = new Node(kv);
			_root->_color = black; // 根节点必须为黑色
			return true;
		}
		else
		{
			Node *cur = _root, *parent = _root;
			while (cur)
			{
				if (kv.first < cur->_kv.first) // key 更小, 向左查找
				{
					parent = cur;
					cur = cur->_left;
				}
				else if (kv.first > cur->_kv.first) // key 更大, 向右查找
				{
					parent = cur;
					cur = cur->_right;
				}
				else // 不可能相等
				{
					return false;
				}
			}
			/* 到这里说明已经找到了合适的位置
			 * 但是不确定最后是在if 中结束的(key  < prev._key )
			 * 还是 else if 中结束的(key  > prev._key )
			 */
			cur = new Node(kv);
			cur->_parent = parent;
			kv.first < parent->_kv.first ? parent->_left = cur : parent->_right = cur;

			// 以上与 BSTree 基本相同, 接下来开始更新节点颜色
			
			if(parent->_color == black) // 父节点黑色, 插入红色的 cur 不会破坏红黑树的规则
				return true;
			
			Node* gparent = parent->_parent; // grand parent
			Node* uncle = nullptr;
			parent == gparent->_left ? uncle = gparent->_right : uncle = gparent->_left;
			/* 第一种情况
			 * uncle 存在且为红, cur 红(必然是新插入的节点), gparent 黑(必然是黑, 并不是一个条件)
			 */
			while(parent->_color == red &&
				(uncle != nullptr && uncle->_color == red))
			{
				parent->_color = black;
				uncle->_color = black;
				gparent->_color = red;
				
				// _root 必须为 黑色, 所以把_root 变回黑色
				// 此时, parent 和 uncle 相当于给所有路径都增加了一个黑色, 所以各路径黑色仍然相等
				if(gparent == _root) 
				{
					gparent->_color = black;
					return true;
				}
				
				// else if(gparent != _root)
				// 继续向上更新节点颜色
				cur = gparent;
				parent = parent->_parent;
				if (parent->_color == black)
					return true;
				gparent = parent->_parent;
				parent == gparent->_left ? uncle = gparent->_right : uncle = gparent->_left;
			}

			/* 第二三种情况
			 * parent 为红, uncle 不存在 或 uncle 存在且为黑, gparent 为黑(必然是黑, 并不是一个条件)
			 * cur为红, 但未新插入或本来就存在, 还是不确定, 以下为两种可能, 但处理方式相同
			 * ** 若 uncle 不存在, 则 cur 必为新插入的顶点
			 * ** 若 uncle 存在且为黑, 则 cur 必为由情况一更新的来
			 */
			if(parent->_color == red &&
				(uncle == nullptr || (uncle != nullptr && uncle->_color == black)))
			{
				if(parent == gparent->_left)
				{
					// 第三种情况, gparent, parent, cur 的路径为折线, 左-右
					// 由于第三种情况经过单次旋转后将会符合情况二的条件, 所以这里把情况三放到前面
					if(cur == parent->_right)
					{
						LtRotate(parent);
						// 旋转完成后, cur 实际上成为了(指向)父节点, parent 成为了(指向)子节点
						// 交换cur 与 parent, 使其逻辑符合事实
						std::swap(cur, parent);
					}

					//至此完全符合情况二
					// 如果从 if(cur == parent->_left) 进来不符合 if(parent == gparent->_right)(情况三), 那就直接来到这里(情况二)
					// 要么经过第三种情况处理来到这里, 要么直接来到这里
					RtRotate(gparent);
					parent->_color = black;
					gparent->_color = red;
					// 到此, 完全消除了 cur 的插入或改变 对gparent 的影响(黑节点数量不变, 无连续红节点), 上面的节点不需要再更新了
					return true;
				}

				if(parent == gparent->_right)
				{
					// 第三种情况, gparent, parent, cur 的路径为折线, 右-左
					// 由于第三种情况经过单次旋转后将会符合情况二的条件, 所以这里把情况三放到前面
					if(cur == parent->_left)
					{
						RtRotate(parent);
						// 旋转完成后, cur 实际上成为了(指向)父节点, parent 成为了(指向)子节点
						// 交换cur 与 parent, 使其逻辑符合事实
						std::swap(cur, parent);
					}

					// 至此完全符合情况二
					// 如果从 if(cur == parent->_right) 进来不符合 if(parent == gparent->_left)(情况三), 那就直接来到这里(情况二)
					// 要么经过第三种情况处理来到这里, 要么直接来到这里
					LtRotate(gparent);
					parent->_color = black;
					gparent->_color = red;
					// 到此, 完全消除了 cur 的插入或改变 对gparent 的影响(黑节点数量不变, 无连续红节点), 上面的节点不需要再更新了
					return true;
				}
			}
		}
		return false;
	}

	template <typename K, typename V>
	void RBTree<K, V>::RtRotate(Node* node)
	{
		Node* parent = node->_parent;
		
		// 左子节点, 可能是调用函数位置的 parent, 也可能是调用位置的 cur
		Node* subL = node->_left; 
		Node* subLR = subL->_right; 

		node->_left = subLR;
		if(subLR != nullptr)
			subLR->_parent = node;
		
		subL->_right = node;
		node->_parent = subL;

		subL->_parent = parent;
		if(node == _root)
			_root = subL;
		else
			node == parent->_left ? parent->_left = subL : parent->_right = subL;
		// 为了复用该函数, 需要将颜色更新写在函数外面
	}

	template <typename K, typename V>
	void RBTree<K, V>::LtRotate(Node* node)
	{
		Node* parent = node->_parent;

		// 右子节点, 可能是调用函数位置的 parent, 也可能是调用位置的 cur
		Node* subR = node->_right;
		Node* subRL = subR->_left;
		
		node->_right = subRL;
		if(subRL != nullptr)
			subRL->_parent = node;
		
		subR->_left = node;
		node->_parent = subR;

		subR ->_parent = parent;
		if(node == _root)
			_root = subR;
		else
			node == parent->_left ? parent->_left = subR : parent->_right = subR;
		// 为了复用该函数, 需要将颜色更新写在函数外面	
	}

	template <typename K, typename V>
	void RBTree<K, V>::__PrintInOrder(Node* node)const
	{
		if(!node)
			return;
		__PrintInOrder(node->_left);
		std::cout << "("<< node->_kv.first  << ", " << node->_kv.second << ")" << " ";
		__PrintInOrder(node->_right);
	}

	template <typename K, typename V>
	void RBTree<K, V>::PrintInOrder()const
	{
		__PrintInOrder(_root);
		std::cout << std::endl;
	}

	template <typename K, typename V>
	void RBTree<K, V>::PrintLevelOrder()const
	{
		if(_root == nullptr)
		{
			std::cout << "Empty Tree" << std::endl;
			return;
		}

		std::queue<Node*> qu;
		Node* node = _root;
		qu.push(node);
		while(!qu.empty())
		{
			// if(node->_left != nullptr)
				qu.push(node->_left);//无论是有效节点或是nullptr, 皆 push
			// else
			// 	qu.push(nullptr);

			// if(node->_right != nullptr)
				qu.push(node->_right);
			// else
			// 	qu.push(nullptr);
			std::cout << "(" << qu.front()->_kv.first << ", " << qu.front()->_kv.second <<  ", " << qu.front()->_color <<" )" << " ";
			qu.pop();
			node = qu.front();
			while(!node)
			{
				std::cout << "nullptr" << " ";
				qu.pop();
				node = qu.front();
			}
		}
		std::cout << std::endl;
	}

	template <typename K, typename V>
	void RBTree<K, V>::TreeHeight()
	{
		std::cout << __TreeMaxHeight(_root) << std::endl;
		std::cout << __TreeMinHeight(_root) << std::endl;
	}
	template <typename K, typename V>
	int RBTree<K, V>::__TreeMaxHeight(Node* node)
	{
		if(node == nullptr)
			return 0;
		int leftHeight = __TreeMaxHeight(node->_left);
		int rightHeight = __TreeMaxHeight(node->_right);
		return leftHeight > rightHeight ? ++leftHeight : ++rightHeight;
	}

		template <typename K, typename V>
	int RBTree<K, V>::__TreeMinHeight(Node* node)
	{
		if(node == nullptr)
			return 0;
		int leftHeight = __TreeMinHeight(node->_left);
		int rightHeight = __TreeMinHeight(node->_right);
		return leftHeight < rightHeight ? ++leftHeight : ++rightHeight;
	}

	template <typename K, typename V>
	bool RBTree<K, V>::isValidRBTree()
	{
		if(_root == nullptr)
			return true; // 空树也是红黑树
		// else if(_root != nullptr)
		// 先求出任意路径的黑色节点的数量, 这里选择最左侧的路径
		Node* cur = _root;
		int basicBlackNum = 0;
		while(cur != nullptr)
		{
			if(cur->_color == black)
				++basicBlackNum;
			cur = cur->_left;
		}
		// 任意路径黑色节点
		// 必须作为参数传入, 否则无法累计, 会重复创建新的临时变量
		// 静态或全局变量也可以
		int anyPathBlackNum = 0; 
		return __isValidRBTree(_root, basicBlackNum, anyPathBlackNum);
	}

	template <typename K, typename V>
	bool RBTree<K, V>::__isValidRBTree(Node* node, int basicBlackNum, int anyPathBlackNum)
	{
		/* 判断个路径黑色节点数是否相等*/
		if(node == nullptr) // 说明该路径已经遍历完成
		{
			if(anyPathBlackNum == basicBlackNum)
				return true;
			else
			{
				std::cout << "黑色节点数量不相等" << std::endl;
				return false;
			}
		}
		if(node->_color == black)
			++anyPathBlackNum;
		
		/*判断是否存在红色连续*/
		if(node->_color == red &&
			node->_parent != nullptr && node->_parent->_color == red)
		{
			std::cout << node->_kv.first <<node->_kv.second << " 当前节点及父节点连续为红色 " << std::endl;
			return false;
		}
		return __isValidRBTree(node->_left, basicBlackNum, anyPathBlackNum) &&
			 __isValidRBTree(node->_right, basicBlackNum, anyPathBlackNum);
	}
}