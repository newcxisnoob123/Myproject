#pragma once
#include <queue>
#include <vector>
#include "FileCompressHuffman.h"
using namespace std;
template <class W>
struct HuffmanTreeNode
{
	HuffmanTreeNode<W>* _left;//左节点
	HuffmanTreeNode<W>* _right;//右节点
	HuffmanTreeNode<W>* _parent;//父节点
	W _weight;//权值
	HuffmanTreeNode(const W&weight)
		:_left(nullptr)
		,_right(nullptr)
		,_parent(nullptr)
		,_weight(weight)
	{}

};

template<class W>
class HuffmanTree
{
	
	typedef HuffmanTreeNode<W> Node;
	class Compare
	{
	public:
		bool operator()(const Node* lf, const Node* rt)const
		{
			return lf->_weight > rt->_weight;
		}

	};
public:
	HuffmanTree()
		:_root(nullptr)
	{}

	
	HuffmanTree(const vector<W>& wv,const W&valid)
	{
		//1.用所有的权值构建只有根节点二叉树森林
		//森林中的二叉树应该使用堆(优先级队列来保存)
		//小堆--------优先级队列默认是大堆(less降序)
		priority_queue<Node*, vector<Node*>, Compare> pq;//用于创建huffman树
		for (auto& ch: wv)
		{
			if(valid!=ch)
			{
				pq.push(new Node(ch));
			}
		}
		while (pq.size() > 1)
		{
			Node* left = pq.top();
			pq.pop();
			Node* right = pq.top();
			pq.pop();
			Node* parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			left->_parent = parent;
			parent->_right = right;
			right->_parent = parent;
			//压入根节点,放入二叉树森林
			pq.push(parent);

		}
		//更新根节点
		_root = pq.top();
	}
	~HuffmanTree()
	{
		Destory(_root);
	}
	Node* GetRoot()
	{
		return _root;
	}
private:
	
	void Destory(Node* root)//后序销毁
	{
		if (nullptr == root)
			return;
		Destory(root->_left);
		Destory(root->_right);
		delete root;
		root = nullptr;
	}
private:
	Node* _root;
};
