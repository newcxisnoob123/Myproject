#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "FileCompressHuffman.h"

void Menu()
{
	cout << "***************************" << endl;
	cout << "*****  0. 退出     ********" << endl;
	cout << "*****  1.压缩文件  *********" << endl;
	cout << "*****  2.解压文件  *********" << endl;
	cout << "***************************" << endl;
}
void Test_compress()
{
	int input;
	do 
	{
		Menu();
		string filePath;
		cout << "请选择以上操作> ";
		cin >> input;
		FileCompressHuffHM fc;
		switch (input)
		{
		case 0:
			cout << "退出成功> " << endl;
			break;
		case 1:
			cout << "请输入要压缩的文件(包含文件格式)> ";
			cin >> filePath;
			fc.CompressFile(filePath);
			//cout << "压缩文件成功！" << endl;
			filePath = "";//置空
			break;
		case 2:
			cout << "输入要解压缩的文件(包含文件格式)> ";
			cin >> filePath;
			fc.UNCompressFile(filePath);
			//cout << "解压缩文件成功" << endl;
			filePath = "";
			break;
		default :
			cout << "此操作不存在请重新输入> ";
			break;
		}
	} while (input);
}

int main()
{
	Test_compress();
	return 0;
}