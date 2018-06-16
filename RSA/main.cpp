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
{//菜单显示函数
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "               e.encrypt 加密              " << endl;
	cout << "               d.decrypt 解密              " << endl;
	cout << "               s.setkey 重置               " << endl;
	cout << "               p.print 显示               " << endl;
	cout << "               q.quit 退出                 " << endl;
	cout << "input your choice:" << endl;
}

bool islegal(const string& str)
{//判断输入是否合法
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it))//不是字母数字
			return false;
	return true;
}

bool decode(Rsa& rsa)
{//解密
	string str;
	do
	{
		cout << ">输入16进制数据:";
		cin >> str;
	} while (cin && str.length()<1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt c(str);

	long t1 = clock();
	BigInt m = rsa.decodeByPr(c);
	long t2 = clock();
	cout << "用时:" << (t2 - t1) << "ms." << endl;

	cout << "密文:" << c << endl
		<< "明文:" << m << endl;
	return true;
}

bool encry(Rsa& rsa, BigInt& cipher_text)
{//加密
	string input_data;
	do
	{
		cout << ">输入16进制数据:";
	} while (!(cin >> input_data));

	if (!islegal(input_data))
		return false;

	BigInt plain_text(input_data);
	cipher_text = rsa.encryptByPu(plain_text);

	cout << "明文:" << plain_text << endl
		<< "密文:" << cipher_text << endl;

	return true;
}

void print(Rsa& rsa)
{//输出
	cout << rsa << endl;
}

void init(Rsa& rsa, int n)
{//初始化

	cout << "初始化...." << endl;
	long t1 = clock();
	rsa.init(n);
	long t2 = clock();
	cout << "初始化完成." << endl;
	cout << "用时:" << (t2 - t1) / 1000 << "s." << endl;
}

//控制函数
int go()
{
	string input;
	Rsa rsa;
	BigInt c, m;
	cout << "输入模数的位数:";
	int prime_number_bits = 0;
	cin >> prime_number_bits;
	init(rsa, prime_number_bits / 2);

	while (true)
	{
		menu();//菜单显示
		cout << ">";
		if (!(cin >> input))
			cerr << "未能成功输入" << endl;

		switch (input[0])
		{
		case 'e':
		case 'E':
			encry(rsa, c);//加密
			break;
		case 'd':
		case 'D':
			decode(rsa);//解密
			break;
		case 's':
		case 'S':
			go();//重新开始初始
			break;
		case 'p':
		case 'P':
			print(rsa);//输出公私钥信息
			break;
		case 'q':
		case 'Q':
			return 0;

		default:
			cout << "错误的输入" << endl;
		}
	}
}


int main()
{
	go();
}