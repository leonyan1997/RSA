#include "stdafx.h"



#include<iostream>
#include<ctime>
#include"BigInt.h"
#include"Rsa.h"
using std::cout;
using std::endl;
using std::cin;
using std::cerr;


void menu()
{//�˵���ʾ����
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "               e.encrypt ����              " << endl;
	cout << "               d.decrypt ����              " << endl;
	cout << "               s.setkey ����               " << endl;
	cout << "               p.print ��ʾ               " << endl;
	cout << "               q.quit �˳�                 " << endl;
	cout << "input your choice:" << endl;
}

bool islegal(const string& str)
{//�ж������Ƿ�Ϸ�
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it))//������ĸ����
			return false;
	return true;
}

bool decode(Rsa& rsa)
{//����
	string str;
	do
	{
		cout << ">����16��������:";
		cin >> str;
	} while (cin && str.length()<1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt c(str);

	long t1 = clock();
	BigInt m = rsa.decodeByPr(c);
	long t2 = clock();
	cout << "��ʱ:" << (t2 - t1) << "ms." << endl;

	cout << "����:" << c << endl
		<< "����:" << m << endl;
	return true;
}

bool encry(Rsa& rsa, BigInt& cipher_text)
{//����
	string input_data;
	do
	{
		cout << ">����16��������:";
	} while (!(cin >> input_data));

	if (!islegal(input_data))
		return false;

	BigInt plain_text(input_data);
	cipher_text = rsa.encryptByPu(plain_text);

	cout << "����:" << plain_text << endl
		<< "����:" << cipher_text << endl;

	return true;
}

void print(Rsa& rsa)
{//���
	cout << rsa << endl;
}

void init(Rsa& rsa, int n)
{//��ʼ��

	cout << "��ʼ��...." << endl;
	long t1 = clock();
	rsa.init(n);
	long t2 = clock();
	cout << "��ʼ�����." << endl;
	cout << "��ʱ:" << (t2 - t1) / 1000 << "s." << endl;
}

//���ƺ���
int go()
{
	string input;
	Rsa rsa;
	BigInt c, m;
	cout << "����ģ����λ��:";
	int prime_number_bits = 0;
	cin >> prime_number_bits;
	init(rsa, prime_number_bits / 2);

	while (true)
	{
		menu();//�˵���ʾ
		cout << ">";
		if (!(cin >> input))
			cerr << "δ�ܳɹ�����" << endl;

		switch (input[0])
		{
		case 'e':
		case 'E':
			encry(rsa, c);//����
			break;
		case 'd':
		case 'D':
			decode(rsa);//����
			break;
		case 's':
		case 'S':
			go();//���¿�ʼ��ʼ
			break;
		case 'p':
		case 'P':
			print(rsa);//�����˽Կ��Ϣ
			break;
		case 'q':
		case 'Q':
			return 0;

		default:
			cout << "���������" << endl;
		}
	}
}


int main()
{
	go();
}