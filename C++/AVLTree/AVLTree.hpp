//仅实现了 insert 函数

#include <iostream>
#include <queue>
#include <utility>
#include <cassert>
#include <cstdlib>

namespace wxy
{
	template <typename K, typename V>
	struct __AVLTreeNode
	{
	public:
		__AVLTreeNode(const std::pair<K, V>& kv)
			: _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr),_bf(0)
		{
		}

	public:
		std::pair<K, V> _kv;
		__AVLTreeNode<K, V> *_left;
		__AVLTreeNode<K, V> *_right;
		__AVLTreeNode<K, V> *_parent;
		int _bf; // 平衡因子, 右子树高度 - 左子树高度
	};

	template <typename K, typename V>
	class AVLTree
	{
	public:
		using Node = __AVLTreeNode<K, V>;

	public:
		AVLTree()
			: _root(nullptr)
		{
		}
		bool Insert(const std::pair<K, V> kv);
		void PrintInOrder()const;
		void PrintLevelOrder()const;
		int TreeHeight(Node* node);
		bool isBalance();
	private:
		void __PrintInOrder(Node* node)const;
		void RtRotate(Node* parent); // parent._bf == -2 || cur->_bf == -1, 即左高 且 直线, 需右单旋
		void LtRotate(Node* parent); // parent._bf == 2 || cur->_bf == 1, 即右高 且 直线, 需左单旋
		void RtLtRotate(Node* parent); // parent._bf == 2 || cur->_bf == -1, parent 左高, cur 右高, 折线, 右左双旋
		void LtRtRotate(Node* parent); // parent._bf == -2 || cur->_bf == 1, parent 右高, cur 左高, 折线, 左右双旋
		bool __isBalance(Node* node);
	private:
		Node *_root;
	};

	template <typename K, typename V>
	bool AVLTree<K, V>::Insert(const std::pair<K, V> kv)
	{
		if (_root == nullptr) // 空树
		{
			_root = new Node(kv);
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
			 * 还是 if else 中结束的(key  > prev._key )
			 */
			cur = new Node(kv);
			cur->_parent = parent;
			kv.first < parent->_kv.first ? parent->_left = cur : parent->_right = cur;

			// 以上与 BSTree 基本相同, 接下来开始更新平衡因子
			while(parent != nullptr)
			{
				if(cur == parent->_left)
				{
					--(parent->_bf);
				}
				else
				{
					++(parent->_bf);
				}
				// 判断是继续向上更新? 停止? 旋转
				if(parent->_bf == 0) //停止更新
				{
					break;
				}
				else if(parent->_bf == 1 || parent->_bf == -1) // 继续向上更新
				{
					cur = parent;
					parent = parent->_parent;
				}
				else
				{
					if (parent->_bf == -2 || cur->_bf == -1) // 左高 且 直线, 需右单旋
					{
						RtRotate(parent);
					}
					// 0 被插入时,可能变成1 或 -1
					// 现在变成了1, 说明是在cur 的右子树插入的
					// parent 变成了2, 则插入的节点必然属于 parent 的右子树, cur 自然也就属于parent 的右子树
					// 总结, cur 为parent 的右子节点, 插入的节点属于cur 的左子树
					else if (parent->_bf == 2 || cur->_bf == 1) // 右高 且 直线, 需左单旋
					{
						LtRotate(parent);
					}
					else if (parent->_bf == -2 || cur->_bf == 1) // parent 左高, cur 右高, 折线, 右左双旋
					{
						RtLtRotate(parent);
					}
					// 0 被插入时,可能变成 1 或 -1
					// 现在变成了-1, 说明是在 cur 的左子树插入的
					// parent 变成了2, 则插入的节点必然属于 parent 的右子树, cur 自然也就属于 parent 的右子树
					// 总结, cur 为parent 的右子节点, 插入的节点属于cur 的左子树
					else if (parent->_bf == 2 || cur->_bf == -1) // parent 右高, cur 左高, 折线, 左右双旋
					{
						LtRtRotate(parent);
					}
					else
					{
						assert(false);
					}
					break;
				}
			}	
			return true;
		}
	}

	template <typename K, typename V>
	void AVLTree<K, V>::RtRotate(Node* parent)
	{
		Node* pparent = parent->_parent;
		Node* subL = parent->_left; // 左子节点, 起始就是调用函数位置的 cur
		Node* subLR = subL->_right; // 左子节点的右子节点, 这个位置 接下来要被 parent 取代

		subL->_right = parent;
		parent->_parent = subL;

		parent->_left = subLR;
		if(subLR != nullptr)
			subLR->_parent = parent;
		
		subL->_parent = pparent; //无论parent 是否为_root
		if(pparent == nullptr) // 即 (旋转之前)parent 是根节点
		{
			_root = subL;
		}
		else // parent 非根节点, 到此为止, pparent, parent 并没有改变
		{
			if(parent == pparent->_left)
			{
				pparent->_left = subL;
			}
			else
			{
				pparent->_right = subL;
			}
		}

		// 更新平衡因子
		parent->_bf = 0;
		subL->_bf = 0;
	}

	template <typename K, typename V>
	void AVLTree<K, V>::LtRotate(Node* parent)
	{
		Node* pparent = parent->_parent;
		Node* subR = parent->_right; // 右子节点, 起始就是调用函数位置的 cur
		Node* subRL = subR->_left; // 右子节点的左子节点, 这个位置 接下来要被 parent 取代

		subR->_left = parent;
		parent->_parent = subR;

		parent->_right = subRL;
		if(subRL != nullptr)
			subRL->_parent = parent;
		
		subR->_parent = pparent; //无论parent 是否为 根节点
		if(pparent == nullptr) // 即 (旋转之前)parent 是根节点
		{
			_root = subR;
		}
		else // parent 非根节点, 到此为止, pparent, parent 并没有改变
		{
			if(parent == pparent->_left)
			{
				pparent->_left = subR;
			}
			else
			{
				pparent->_right = subR;
			}
		}

		// 更新平衡因子
		parent->_bf = 0;
		subR->_bf = 0;
	}

	template <typename K, typename V>
	void AVLTree<K, V>::RtLtRotate(Node* parent)
	{
		Node* subR = parent->_right; // 右子节点, 调用位置的 cur
		// 右子节点的左子节点
		// subRL 的左右子中,一个高度为 h, 一个为 h-1, 但不确定哪个更高
		Node* subRL = subR->_left;
		int bfSubRL = subRL->_bf;
		
		RtRotate(subR);
		LtRotate(parent);

		/* 由于 subRL 的左右子树不确定, 导致上面右左单旋后 bf 的调整不合理
		 * 从结果来看
		 * ** parent 和 subR 的结果可能是对的
		 * ** ** 即使有些结果是对的, 也只是碰巧对了, 过程逻辑是错误的
		 * ** subRL._bf 的结果一定是对的, 但过程逻辑同样是错误的
		 */
		subRL->_bf = 0; // 即使其是对的, 这里也强调一次
		if(bfSubRL == -1) // subRL 的左子树更高, 即左子树为 h, 右子树为 h -  1
		{
			parent->_bf = 0;
			subR->_bf = 1;
		}
		else // (bf == 1) // 即左子树为 h - 1, 右子树为 h
		{
			parent->_bf = -1;
			subR->_bf = 0;
		}
	}

	template <typename K, typename V>
	void AVLTree<K, V>::LtRtRotate(Node* parent)
	{
		Node* subL = parent->_left; // 左子节点, 调用位置的 cur
		// 左子节点的右子节点
		// subLR 的左右子树中,一个高度为 h, 一个为 h-1, 但不确定哪个更高
		Node* subLR = subL->_right;
		int bfSubLR = subLR->_bf;
		
		LtRotate(subL);
		RtRotate(parent);
		
		/* 由于 subLR 的左右子树不确定, 导致上面左右单旋后 bf 的调整不合理
		 * 从结果来看
		 * ** parent 和 subL 的结果可能是对的
		 * ** ** 即使有些结果是对的, 也只是碰巧对了, 过程逻辑是错误的
		 * ** subLR._bf 的结果一定是对的, 但过程逻辑同样是错误的
		 */
		subLR->_bf = 0; // 即使其是对的, 这里也强调一次
		if(bfSubLR == -1) // subLR 的左子树更高, 即左子树为 h, 右子树为 h -  1
		{
			parent->_bf = 1;
			subL->_bf = 0;
		}
		else // (bf == 1) // 即左子树为 h - 1, 右子树为 h
		{
			parent->_bf = 0;
			subL->_bf = -1;
		}
	}

	template <typename K, typename V>
	void AVLTree<K, V>::__PrintInOrder(Node* node)const
	{
		if(!node)
			return;
		__PrintInOrder(node->_left);
		std::cout << "("<< node->_kv.first  << ", " << node->_kv.second << ")" << " ";
		__PrintInOrder(node->_right);
	}

	template <typename K, typename V>
	void AVLTree<K, V>::PrintInOrder()const
	{
		__PrintInOrder(_root);
		std::cout << std::endl;
	}

	template <typename K, typename V>
	void AVLTree<K, V>::PrintLevelOrder()const
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
			std::cout << "(" << qu.front()->_kv.first << ", " << qu.front()->_kv.second << ")" << " ";
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
	int AVLTree<K, V>::TreeHeight(Node* node)
	{
		if(node == nullptr)
			return 0;
		int leftHeight = TreeHeight(node->_left);
		int rightHeight = TreeHeight(node->_right);
		return leftHeight > rightHeight ? ++leftHeight : ++rightHeight;
	}

	template <typename K, typename V>
	bool AVLTree<K, V>::isBalance()
	{
		return __isBalance(_root);
	}

	template <typename K, typename V>
	bool AVLTree<K, V>::__isBalance(Node* node)
	{
		if(node == nullptr)
			return true;
		
		int leftHeight = TreeHeight(node->_left);
		int rightHeight = TreeHeight(node->_right);
		int difer = rightHeight - leftHeight;
		if (abs(difer) >= 2)
		{
			std::cout << "(" << node->_kv.first << ", " << node->_kv.second << ") " << "_bf 异常" << std::endl;
			return false;
		}
		if (difer != node->_bf)
		{
			std::cout << "(" << node->_kv.first << ", " << node->_kv.second << ") " << "_bf 与实际不符" << std::endl;
			return false;
		}
		return __isBalance(node->_left) && __isBalance(node->_right);
	}
}