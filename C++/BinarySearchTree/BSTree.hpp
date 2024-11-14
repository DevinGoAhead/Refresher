#include <iostream>
#include <queue>

namespace wxy
{
	template <typename K, typename V>
	struct __BSTreeNode
	{
	public:
		__BSTreeNode(const K &key = K(), const V &val = V())
			: _key(key), _val(val), _left(nullptr), _right(nullptr)
		{
		}

	public:
		K _key;
		V _val;
		__BSTreeNode<K, V> *_left;
		__BSTreeNode<K, V> *_right;
	};

	template <typename K, typename V>
	class BSTree
	{
	public:
		using Node = __BSTreeNode<K, V>;

	public:
		BSTree()
			: _root(nullptr)
		{
		}
		bool Insert(const K &key, const V & val);
		bool Erase(const K &key);
		Node* Find(const K &key);
		void PrintInOrder()const;
		void PrintLevelOrder()const;
	private:
		void __PrintInOrder(Node* node)const;
	private:
		Node *_root;
	};

	template <typename K, typename V>
	bool BSTree<K, V>::Insert(const K &key, const V & val)
	{
		if (_root == nullptr) // 空树
		{
			_root = new Node(key, val);
			return true;
		}
		else
		{
			Node *cur = _root, *prev = _root;
			while (cur)
			{
				if (key < cur->_key) // key 更小, 向左查找
				{
					prev = cur;
					cur = cur->_left;
				}
				else if (key > cur->_key) // key 更大, 向右查找
				{
					prev = cur;
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
			key < prev->_key ? prev->_left = new Node(key, val) : prev->_right = new Node(key, val);
			return true;
		}
	}

	template <typename K, typename V>
	bool BSTree<K, V>::Erase(const K &key)
	{
		if (_root == nullptr)
			return false;
		else
		{
			//先找到这个值
			Node *cur = _root, *prev = _root;
			while (cur)
			{
				if (key < cur->_key) // key 更小, 向左查找
				{
					prev = cur;
					cur = cur->_left;
				}
				else if (key > cur->_key) // key 更大, 向右查找
				{
					prev = cur;
					cur = cur->_right;
				}
				else // key == cur->_key
				{
					break;
				}
			}
			if(cur == nullptr) // 没找到 key == cur->_key
				return false;
			else // 找到了 key == cur->_key
			{
				if (cur->_right == nullptr) // cur 仅有左子节点 或 左右子节点均为空
				{
					if(cur->_key < prev->_key) // cur 为左子节点
					{
						prev->_left = cur->_left;
						delete cur;
						return true;
					}
					if(cur->_key > prev->_key) //cur 为右子节点
					{
						prev->_right = cur->_left;
						delete cur;
						return true;
					}
				}
				else if (cur->_left == nullptr) // cur 仅有右子节点
				{
					if(cur->_key < prev->_key) //cur 为左子节点
					{
						prev->_left = cur->_right;
						delete cur;
						return true;
					}
					if(cur->_key > prev->_key) //cur 为右子节点
					{
						prev->_right = cur->_right;
						delete cur;
						return true;
					}
				}
				else // cur 有两个子节点
				{
					/* 尝试在 cur 的左子树找最大值
					 * 当然也可以在 cur 的右子树寻找最小值, 逻辑相同
					 */
					Node* delNode = cur;
					prev = cur;
					cur = cur->_left;
					while(cur->_right)
					{
						prev = cur;
						cur = cur->_right;
					}
					// 进入循环前 prev == cur, cur == cur->_left, 故 cur == prev->_left
					// 进入循环后 prev == cur, cur == cur->_right, 故 cur == prev->_right
					// 也可以使用 cur != del->_left 判断
					// 进入了循环, 说明左树是存在最大值的(本质是 delNode 的左子节点存在右树)
					if(cur == prev->_right) 
					{
						delNode->_key = cur->_key;
						delNode->_val = cur->_val;
						prev->_right = cur->_left; // 无论cur 是否有左子节点, 均适用
						delete cur;
						return true;
					}
					else // 没进入循环, 左树不存在最大值(本质是 delNode 的左子节点不存在右树)
					{
						delNode->_key = cur->_key;
						delNode->_val = cur->_val;
						prev->_left = cur->_left;
						delete cur;
						return true;
					}
				}
			}
		}
	}

	template <typename K, typename V>
	typename BSTree<K, V>::Node* BSTree<K, V>::Find(const K &key)
	{
		//先找到这个值
		Node *cur = _root, *prev = _root;
		while (cur)
		{
			if (key < cur->_key) // key 更小, 向左查找
			{
				prev = cur;
				cur = cur->_left;
			}
			else if (key > cur->_key) // key 更大, 向右查找
			{
				prev = cur;
				cur = cur->_right;
			}
			else // key == cur->_key
			{
				return cur;
			}
		}
		return nullptr;
	}

	template <typename K, typename V>
	void BSTree<K, V>::__PrintInOrder(Node* node)const
	{
		if(!node)
			return;
		__PrintInOrder(node->_left);
		std::cout << "("<< node->_key  << ", " << node->_val << ")" << " ";
		__PrintInOrder(node->_right);
	}

	template <typename K, typename V>
	void BSTree<K, V>::PrintInOrder()const
	{
		__PrintInOrder(_root);
		std::cout << std::endl;
	}

	template <typename K, typename V>
	void BSTree<K, V>::PrintLevelOrder()const
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
			std::cout << "(" << qu.front()->_key << ", " << qu.front()->_val << ")" << " ";
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
}