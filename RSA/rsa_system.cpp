#include "stdafx.h"

#include "rsa_system.h"



//菜单显示函数
void RsaSystem::menu()
{
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "               e.encrypt 加密              " << endl;
	cout << "               d.decrypt 解密              " << endl;
	cout << "               s.setkey 重置               " << endl;
	cout << "               p.print 显示               " << endl;
	cout << "               q.quit 退出                 " << endl;
	cout << "input your choice:" << endl;
}

bool RsaSystem::isLegal(const string& str)
{//判断输入是否合法
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it))//不是字母数字
			return false;
	return true;
}
//解密
bool RsaSystem::decode(Rsa& rsa)
{
	string input_cipher_text;
	do
	{
		cout << ">输入16进制密文:";
	} while (!(cin >> input_cipher_text));

	if (!isLegal(input_cipher_text))
		return false;

	LargeInt cipher_text(input_cipher_text);

	long t1 = clock();
	LargeInt plain_text = rsa.Decode(cipher_text);
	long t2 = clock();
	cout << "用时:" << (t2 - t1) << "ms." << endl;

	cout << "密文:" << cipher_text << endl
		<< "明文:" << plain_text << endl;
	return true;
}
//加密
bool RsaSystem::encrypt(Rsa& rsa)
{
	string input_plain_text;
	do
	{
		cout << ">输入16进制明文:";
	} while (!(cin >> input_plain_text));

	if (!isLegal(input_plain_text))
		return false;

	LargeInt plain_text(input_plain_text);
	LargeInt cipher_text = rsa.Encrypt(plain_text);

	cout << "明文:" << plain_text << endl
		<< "密文:" << cipher_text << endl;

	return true;
}

void RsaSystem::print(Rsa& rsa)
{//输出
	cout << rsa << endl;
}

//控制函数
void RsaSystem::Start()
{
	string input_choice;
	int modulus_bits = 0;

	cout << "输入模数的位数:";
	cin >> modulus_bits;
	int prime_number_length = modulus_bits / 2;

	cout << "初始化...." << endl;
	long t1 = clock();
	Rsa rsa(prime_number_length);
	long t2 = clock();
	cout << "初始化完成." << endl;
	cout << "用时:" << (t2 - t1) << "ms." << endl;

	while (true)
	{
		menu();//菜单显示
		cout << ">";
		if (!(cin >> input_choice))
			cerr << "未能成功输入" << endl;

		switch (input_choice[0])
		{
		case 'e':
		case 'E':
			encrypt(rsa);//加密
			break;
		case 'd':
		case 'D':
			decode(rsa);//解密
			break;
		case 's':
		case 'S':
			Start();//重新开始初始
			break;
		case 'p':
		case 'P':
			print(rsa);//输出公私钥信息
			break;
		case 'q':
		case 'Q':
			return;

		default:
			cerr << "错误的输入" << endl;
		}
	}
}