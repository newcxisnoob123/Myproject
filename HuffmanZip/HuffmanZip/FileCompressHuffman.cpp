#define _CRT_SECURE_NO_WARNINGS
#include "FileCompressHuffman.h"
#include "HuffmanTree.h"
#include <iostream>
#include <vector>
FileCompressHuffHM::FileCompressHuffHM()
{
	_fileInfo.resize(256);
	for (size_t i = 0;i < 256;i++)
	{
		_fileInfo[i]._ch = i;//这里发生隐式类型转换 int-->char
	}
}
void FileCompressHuffHM::CompressFile(const string& filePath)//压缩文件
{
	//1.统计源文件中每个字符出现的次数
	FILE* FIn = fopen(filePath.c_str(), "rb");//以读的方式打开源文件
	if (nullptr == FIn)//文件打开失败
	{
		cout << "文件打开失败" << endl;
		return;
	}
	//说打开文件成功
	//读取文件
	uch rdBuff[1024];//存储每次读取文件的数据(1k)
	while (true)
	{
		size_t rdSize = fread(rdBuff, 1, 1024, FIn);
		if (0 == rdSize)
			break;
		for (size_t i = 0;i < rdSize;i++)
		{
			_fileInfo[rdBuff[i]]._appearCount++;//对应字符出现次数++
			//这里会发生隐式类型转换char->int
		}
	}
	//2.用统计的结果构造huffman树
	ByteInfo valid;
	HuffmanTree<ByteInfo> ht(_fileInfo, valid);
	//3.获取huffman编码
	GenerateHuffmanCode(ht.GetRoot());
	fseek(FIn, 0, SEEK_SET);//让文件流指向文件头部
	FILE* FOut = fopen("compressed.hz", "wb");//以写的形式打开文件，若这个文件未找到就会创建一个新的文件

	//4.写用来解压缩的信息
	WriteHeadInfo(filePath, FOut);

	//5.用获取的编码对源文件进行改写
	uch bits = 0;
	int bitCount = 0;//记录一个字节8个bite是否填充满了
	while (true)
	{
		size_t rdSize=fread(rdBuff, 1, 1024, FIn);
		if (0 == rdSize)//文件读取完毕
			break;
		for (size_t i = 0;i < rdSize;++i)
		{
			string & strCode=_fileInfo[rdBuff[i]]._chCode;//获取每个字节rdBuff里面huffman编码
			for (int j = 0;j < strCode.size();++j)
			{
				bits <<= 1;
				if ('1' == strCode[j])
				{
					bits |= 1;
				}
				bitCount++;
				if (8 == bitCount)
				{
					fputc(bits, FOut);
					bits = 0;
					bitCount = 0;
				}
			}
		}

	}
	//最后一次bits中的8个比特位可能没有填充满
	if (bitCount > 0 && bitCount < 8)
	{
		bits <<= (8 - bitCount);
		fputc(bits, FOut);
	}
	//关闭文件
	fclose(FIn);
	fclose(FOut);
}

void FileCompressHuffHM::WriteHeadInfo(const string& filePath, FILE* FOut)//写文件的一些头部信息
{
	//1.获取源文件的后缀
	string headInfo;
	headInfo += GetFileSuffix(filePath);
	headInfo += '\n';

	//2.构造频次信息
	size_t LineCount=0;//统计行数信息
	string chInfo;//每个字符的信息
	for (auto &ch : _fileInfo)
	{
		if (0 == ch._appearCount)
			continue;
		++LineCount;
		chInfo += ch._ch;
		chInfo += ":";
		chInfo += to_string(ch._appearCount);
		chInfo += '\n';
	}
	headInfo += to_string(LineCount);
	headInfo += '\n';

	//3.写头部信息
	fwrite(headInfo.c_str(), 1, headInfo.size(), FOut);//size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
	fwrite(chInfo.c_str(), 1, chInfo.size(), FOut);

}
string FileCompressHuffHM::GetFileSuffix(const string& filePath)//获取文件的后缀
{
	//string suffix;
	//return  suffix+=filePath.substr(filePath.find_last_not_of('.') + 1);//返回从最后一个'.'开始的子串
	return filePath.substr(filePath.find_last_of('.') + 1);
}

void FileCompressHuffHM::GenerateHuffmanCode(HuffmanTreeNode<ByteInfo>* root)//获取huffman编码
{
	if (nullptr == root)
		return;
	GenerateHuffmanCode(root->_left);
	GenerateHuffmanCode(root->_right);
	
	if (nullptr == root->_left && nullptr == root->_right)//找到叶子节点
	{
		string& chCode=_fileInfo[root->_weight._ch]._chCode;
		HuffmanTreeNode<ByteInfo>* cur = root;
		HuffmanTreeNode<ByteInfo>* parent = cur->_parent;
		while (parent)
		{
			if (parent->_left == cur)//左子节点
				chCode += '0';
			else
				chCode += '1';
			cur = parent;
			parent = cur->_parent;
		}
		reverse(chCode.begin(), chCode.end());
	}
}

void FileCompressHuffHM::GetLine(FILE* FIn, string& strInfo) //按行读取信息
{
	while (!feof(FIn))//判断文件是否读取到结尾
	{
		char ch = fgetc(FIn);//每次读取一个字符
		if (ch == '\n')//说明读取一行了
			break;
		strInfo += ch;
	}
}


void FileCompressHuffHM::UNCompressFile(const string& filePath)//解压缩文件
{
	//1.判定解压缩文件的格式是否有问题
	if (GetFileSuffix(filePath) != "hz")
	{
		cout << "文件的格式不对" << endl;
		return;
	}
	//2.读解压缩需要用到的文件信息
	FILE* FIn = fopen(filePath.c_str(), "rb");
	if (nullptr == FIn)
	{
		cout << "需要解压缩的文件不存在/路径不对" << endl;
		return;
	}
	//a.读取文件的后缀
	string CompressFileName("UNCompressed.");//保存解压缩后的文件名字
	//string strInfo = GetFileSuffix(filePath);//获取文件后缀
	string strInfo;
	GetLine(FIn, strInfo);
	CompressFileName += strInfo;

	//b.读取频次信息的总行数
	strInfo = "";//清空数据
	GetLine(FIn, strInfo);
	size_t LineCount = 0;
	 LineCount+= atoi(strInfo.c_str());//atoi
	strInfo = "";//清空数据

	//c.读取字符及对应频次信息
	//GetLine(FIn, strInfo);
	for (size_t i = 0;i < LineCount;i++)
	{
		
		strInfo = "";
		GetLine(FIn, strInfo);
		//换行需要特殊处理
		if ("" == strInfo)
		{
			strInfo += "\n";
			GetLine(FIn, strInfo);
		}
		//A:1----->_fileInfo
		uch ch = strInfo[0];
		_fileInfo[ch]._ch = ch;
		_fileInfo[ch]._appearCount = atoi(strInfo.c_str() + 2);
	}

	//3.还原haffman树
	HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());

	//4.解压缩
	FILE* FOut = fopen(CompressFileName.c_str(), "wb");
	uch rdBuff[1024];
	auto cur = ht.GetRoot();
	auto parent = cur->_parent;
	size_t fileSize = 0;//每个字符出现的次数
	while (true)
	{
		size_t rdSize = fread(rdBuff, 1, 1024, FIn);
		if (0 == rdSize)
			break;
		for (size_t i = 0;i < rdSize;++i)
		{
			char ch = rdBuff[i];
			for (int j = 0;j < 8;++j)
			{
				if (ch & 0x80)//1(每次都和最高位比较，==1，往右走，否则往左走)
					cur = cur->_right;
				else
					cur = cur->_left;

				ch <<= 1;//把高位的移走
				if (nullptr == cur->_left && nullptr == cur->_right)//走到根节点
				{
					fputc(cur->_weight._ch, FOut);
					cur = ht.GetRoot();
					++fileSize;
					if (fileSize == cur->_weight._appearCount)
					{
						break;
						fileSize = 0;
					}
				}
			}
		}

	}
	fclose(FIn);
	fclose(FOut);
}