#pragma once
#include <queue>
#include <vector>
#include <string>
#include "common.h"
#include "HuffmanTree.h"
struct ByteInfo//�洢�����ֽڵ������Ϣ
{
	uch _ch;//�����ַ�����
	size_t _appearCount;//�����ַ����ִ���
	string _chCode;//�洢huffman����

	ByteInfo(size_t appearCount = 0)
		:_appearCount(appearCount)
	{}
	ByteInfo operator+(const ByteInfo& other)const
	{
		return ByteInfo(_appearCount + other._appearCount);
	}
	bool operator>(const ByteInfo& other)const
	{
		return _appearCount > other._appearCount;
	}
	bool operator!=(const ByteInfo& other)const
	{
		return _appearCount != other._appearCount;
	}
	bool operator==(const ByteInfo& other)const
	{
		return _appearCount == other._appearCount;
	}

};

class FileCompressHuffHM
{
public:
	FileCompressHuffHM();
	void CompressFile(const string& filePath);//ѹ���ļ�
	void UNCompressFile(const string& filePath);//��ѹ���ļ�

private:
	void WriteHeadInfo(const string& filePath, FILE* FIn);//д�ļ���һЩͷ����Ϣ
	string GetFileSuffix(const string& filePath);//��ȡ�ļ��ĺ�׺
	void GetLine(FILE* FIn, string& strInfo); //���ж�ȡ��Ϣ
private:
	void GenerateHuffmanCode(HuffmanTreeNode<ByteInfo>* root);//��ȡhuffman����
	vector<ByteInfo>_fileInfo;
};