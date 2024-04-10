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
		_fileInfo[i]._ch = i;//���﷢����ʽ����ת�� int-->char
	}
}
void FileCompressHuffHM::CompressFile(const string& filePath)//ѹ���ļ�
{
	//1.ͳ��Դ�ļ���ÿ���ַ����ֵĴ���
	FILE* FIn = fopen(filePath.c_str(), "rb");//�Զ��ķ�ʽ��Դ�ļ�
	if (nullptr == FIn)//�ļ���ʧ��
	{
		cout << "�ļ���ʧ��" << endl;
		return;
	}
	//˵���ļ��ɹ�
	//��ȡ�ļ�
	uch rdBuff[1024];//�洢ÿ�ζ�ȡ�ļ�������(1k)
	while (true)
	{
		size_t rdSize = fread(rdBuff, 1, 1024, FIn);
		if (0 == rdSize)
			break;
		for (size_t i = 0;i < rdSize;i++)
		{
			_fileInfo[rdBuff[i]]._appearCount++;//��Ӧ�ַ����ִ���++
			//����ᷢ����ʽ����ת��char->int
		}
	}
	//2.��ͳ�ƵĽ������huffman��
	ByteInfo valid;
	HuffmanTree<ByteInfo> ht(_fileInfo, valid);
	//3.��ȡhuffman����
	GenerateHuffmanCode(ht.GetRoot());
	fseek(FIn, 0, SEEK_SET);//���ļ���ָ���ļ�ͷ��
	FILE* FOut = fopen("compressed.hz", "wb");//��д����ʽ���ļ���������ļ�δ�ҵ��ͻᴴ��һ���µ��ļ�

	//4.д������ѹ������Ϣ
	WriteHeadInfo(filePath, FOut);

	//5.�û�ȡ�ı����Դ�ļ����и�д
	uch bits = 0;
	int bitCount = 0;//��¼һ���ֽ�8��bite�Ƿ��������
	while (true)
	{
		size_t rdSize=fread(rdBuff, 1, 1024, FIn);
		if (0 == rdSize)//�ļ���ȡ���
			break;
		for (size_t i = 0;i < rdSize;++i)
		{
			string & strCode=_fileInfo[rdBuff[i]]._chCode;//��ȡÿ���ֽ�rdBuff����huffman����
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
	//���һ��bits�е�8������λ����û�������
	if (bitCount > 0 && bitCount < 8)
	{
		bits <<= (8 - bitCount);
		fputc(bits, FOut);
	}
	//�ر��ļ�
	fclose(FIn);
	fclose(FOut);
}

void FileCompressHuffHM::WriteHeadInfo(const string& filePath, FILE* FOut)//д�ļ���һЩͷ����Ϣ
{
	//1.��ȡԴ�ļ��ĺ�׺
	string headInfo;
	headInfo += GetFileSuffix(filePath);
	headInfo += '\n';

	//2.����Ƶ����Ϣ
	size_t LineCount=0;//ͳ��������Ϣ
	string chInfo;//ÿ���ַ�����Ϣ
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

	//3.дͷ����Ϣ
	fwrite(headInfo.c_str(), 1, headInfo.size(), FOut);//size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
	fwrite(chInfo.c_str(), 1, chInfo.size(), FOut);

}
string FileCompressHuffHM::GetFileSuffix(const string& filePath)//��ȡ�ļ��ĺ�׺
{
	//string suffix;
	//return  suffix+=filePath.substr(filePath.find_last_not_of('.') + 1);//���ش����һ��'.'��ʼ���Ӵ�
	return filePath.substr(filePath.find_last_of('.') + 1);
}

void FileCompressHuffHM::GenerateHuffmanCode(HuffmanTreeNode<ByteInfo>* root)//��ȡhuffman����
{
	if (nullptr == root)
		return;
	GenerateHuffmanCode(root->_left);
	GenerateHuffmanCode(root->_right);
	
	if (nullptr == root->_left && nullptr == root->_right)//�ҵ�Ҷ�ӽڵ�
	{
		string& chCode=_fileInfo[root->_weight._ch]._chCode;
		HuffmanTreeNode<ByteInfo>* cur = root;
		HuffmanTreeNode<ByteInfo>* parent = cur->_parent;
		while (parent)
		{
			if (parent->_left == cur)//���ӽڵ�
				chCode += '0';
			else
				chCode += '1';
			cur = parent;
			parent = cur->_parent;
		}
		reverse(chCode.begin(), chCode.end());
	}
}

void FileCompressHuffHM::GetLine(FILE* FIn, string& strInfo) //���ж�ȡ��Ϣ
{
	while (!feof(FIn))//�ж��ļ��Ƿ��ȡ����β
	{
		char ch = fgetc(FIn);//ÿ�ζ�ȡһ���ַ�
		if (ch == '\n')//˵����ȡһ����
			break;
		strInfo += ch;
	}
}


void FileCompressHuffHM::UNCompressFile(const string& filePath)//��ѹ���ļ�
{
	//1.�ж���ѹ���ļ��ĸ�ʽ�Ƿ�������
	if (GetFileSuffix(filePath) != "hz")
	{
		cout << "�ļ��ĸ�ʽ����" << endl;
		return;
	}
	//2.����ѹ����Ҫ�õ����ļ���Ϣ
	FILE* FIn = fopen(filePath.c_str(), "rb");
	if (nullptr == FIn)
	{
		cout << "��Ҫ��ѹ�����ļ�������/·������" << endl;
		return;
	}
	//a.��ȡ�ļ��ĺ�׺
	string CompressFileName("UNCompressed.");//�����ѹ������ļ�����
	//string strInfo = GetFileSuffix(filePath);//��ȡ�ļ���׺
	string strInfo;
	GetLine(FIn, strInfo);
	CompressFileName += strInfo;

	//b.��ȡƵ����Ϣ��������
	strInfo = "";//�������
	GetLine(FIn, strInfo);
	size_t LineCount = 0;
	 LineCount+= atoi(strInfo.c_str());//atoi
	strInfo = "";//�������

	//c.��ȡ�ַ�����ӦƵ����Ϣ
	//GetLine(FIn, strInfo);
	for (size_t i = 0;i < LineCount;i++)
	{
		
		strInfo = "";
		GetLine(FIn, strInfo);
		//������Ҫ���⴦��
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

	//3.��ԭhaffman��
	HuffmanTree<ByteInfo> ht(_fileInfo, ByteInfo());

	//4.��ѹ��
	FILE* FOut = fopen(CompressFileName.c_str(), "wb");
	uch rdBuff[1024];
	auto cur = ht.GetRoot();
	auto parent = cur->_parent;
	size_t fileSize = 0;//ÿ���ַ����ֵĴ���
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
				if (ch & 0x80)//1(ÿ�ζ������λ�Ƚϣ�==1�������ߣ�����������)
					cur = cur->_right;
				else
					cur = cur->_left;

				ch <<= 1;//�Ѹ�λ������
				if (nullptr == cur->_left && nullptr == cur->_right)//�ߵ����ڵ�
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