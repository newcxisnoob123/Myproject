#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "FileCompressHuffman.h"

void Menu()
{
	cout << "***************************" << endl;
	cout << "*****  0. �˳�     ********" << endl;
	cout << "*****  1.ѹ���ļ�  *********" << endl;
	cout << "*****  2.��ѹ�ļ�  *********" << endl;
	cout << "***************************" << endl;
}
void Test_compress()
{
	int input;
	do 
	{
		Menu();
		string filePath;
		cout << "��ѡ�����ϲ���> ";
		cin >> input;
		FileCompressHuffHM fc;
		switch (input)
		{
		case 0:
			cout << "�˳��ɹ�> " << endl;
			break;
		case 1:
			cout << "������Ҫѹ�����ļ�(�����ļ���ʽ)> ";
			cin >> filePath;
			fc.CompressFile(filePath);
			//cout << "ѹ���ļ��ɹ���" << endl;
			filePath = "";//�ÿ�
			break;
		case 2:
			cout << "����Ҫ��ѹ�����ļ�(�����ļ���ʽ)> ";
			cin >> filePath;
			fc.UNCompressFile(filePath);
			//cout << "��ѹ���ļ��ɹ�" << endl;
			filePath = "";
			break;
		default :
			cout << "�˲�������������������> ";
			break;
		}
	} while (input);
}

int main()
{
	Test_compress();
	return 0;
}