#pragma once
#include <queue>
#include <vector>
#include "FileCompressHuffman.h"
using namespace std;
template <class W>
struct HuffmanTreeNode
{
	HuffmanTreeNode<W>* _left;//��ڵ�
	HuffmanTreeNode<W>* _right;//�ҽڵ�
	HuffmanTreeNode<W>* _parent;//���ڵ�
	W _weight;//Ȩֵ
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
		//1.�����е�Ȩֵ����ֻ�и��ڵ������ɭ��
		//ɭ���еĶ�����Ӧ��ʹ�ö�(���ȼ�����������)
		//С��--------���ȼ�����Ĭ���Ǵ��(less����)
		priority_queue<Node*, vector<Node*>, Compare> pq;//���ڴ���huffman��
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
			//ѹ����ڵ�,���������ɭ��
			pq.push(parent);

		}
		//���¸��ڵ�
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
	
	void Destory(Node* root)//��������
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
