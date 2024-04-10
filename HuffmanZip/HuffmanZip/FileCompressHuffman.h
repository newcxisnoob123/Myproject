#pragma once
#include <queue>
#include <vector>
#include <string>
#include "common.h"
#include "HuffmanTree.h"
struct ByteInfo//存储单个字节的相关信息
{
	uch _ch;//单个字符名称
	size_t _appearCount;//单个字符出现次数
	string _chCode;//存储huffman编码

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
	void CompressFile(const string& filePath);//压缩文件
	void UNCompressFile(const string& filePath);//解压缩文件

private:
	void WriteHeadInfo(const string& filePath, FILE* FIn);//写文件的一些头部信息
	string GetFileSuffix(const string& filePath);//获取文件的后缀
	void GetLine(FILE* FIn, string& strInfo); //按行读取信息
private:
	void GenerateHuffmanCode(HuffmanTreeNode<ByteInfo>* root);//获取huffman编码
	vector<ByteInfo>_fileInfo;
};