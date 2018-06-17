#include "stdafx.h"

#include "rsa.h"
#include<iostream>
#include<sstream>
#include<ctime>
#include<cstdlib>

using std::cout;
using std::endl;
using std::ostringstream;

Rsa::Rsa(NumLength prime_number_length)
{
	//产生随机数种
	srand(time(0));
	//产生大素数p、q
	prime_number1 = createPrime(prime_number_length);
	prime_number2 = createPrime(prime_number_length);

	modulus_ = prime_number1 * prime_number2;

	euler_function_num_ = (prime_number1 - 1)*(prime_number2 - 1);

	public_key_ = get_public_key(prime_number_length);
								   
	private_key_ = get_private_key();
}

Rsa::~Rsa()
{
}
/*
void Rsa::Init(unsigned int prime_number_length)
{
	srand(time(NULL));
	//产生大素数p、q
	prime_number1 = createPrime(prime_number_length, 10);
	prime_number2 = createPrime(prime_number_length, 10);

	modulus_ = prime_number1 * prime_number2;

	euler_function_num_ = (prime_number1 - 1)*(prime_number2 - 1);

	public_key_ = get_public_key();//TODO:公钥居然是固定的？！！
	//get_public_key(euler_function_num_);
	private_key_ = get_private_key();
}*/
//生成长度为test_num的奇数
LargeInt Rsa::createOddNum(NumLength odd_num_length)
{
	odd_num_length /= 4;//因为从2进制转化到16进制，位数要变为原先的1/4
	static unsigned char hex_table[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (odd_num_length)
	{
		ostringstream oss;
		for (uint64_t i = 0; i<odd_num_length - 1; ++i)
			oss << hex_table[rand() % 16];
		oss << hex_table[1];
		string str(oss.str());
		return LargeInt(str);
	}
	else
		return LargeInt::Zero;
}
//判断素数，米勒拉宾检验
bool Rsa::isPrime(const LargeInt& test_num)
{
	const int times_counter = 10;

	if (test_num == LargeInt::Two)
		return true;

	LargeInt num_1 = test_num - 1;
	LargeInt::bit bit_num_1(num_1);//二进制的num_1
	if (bit_num_1.at(0) == 1)//最低bit位是1，说明是奇数，在未减1前则是偶数，且不是2。所以返回false。
		return false;

	for (uint64_t t = 0; t<times_counter; ++t)
	{
		LargeInt random_num = createRandomSmallThan(num_1);//随机数
		LargeInt result(LargeInt::One);
		for (int i = bit_num_1.get_size() - 1; i >= 0; --i)
		{
			LargeInt x = result;
			result = (result*result) % test_num;
			if (result == LargeInt::One && x != LargeInt::One && x != num_1)
				return false;

			if (bit_num_1.at(i))
			{
				result = (random_num * result) % test_num;
			}
		}
		if (result != LargeInt::One)
			return false;
	}
	return true;
}

LargeInt Rsa::createRandomSmallThan(const LargeInt & value)
{
	LargeInt random_num = rand();
	random_num = random_num % value;
	if (random_num == 0)
		random_num = value - 1;
	return random_num;
}
//最后一个plain_tesxt的值最小
PlainTextList Rsa::groupPlainText(PlainText plain_text)
{
	PlainTextList plain_text_list;
	while (modulus_.smallOrEquals(plain_text))
	{
		plain_text = plain_text - modulus_;
		plain_text_list.push_back(plain_text);
	}
	plain_text_list.push_back(plain_text);
	return plain_text_list;
}

CipherText Rsa::mergeCipherTextList(const CipherTextList &)
{
	return CipherText();
}
//生成长度为test_num的素数
LargeInt Rsa::createPrime(NumLength prime_number_length)
{
	LargeInt odd_num = createOddNum(prime_number_length);
	while (!isPrime(odd_num))
	{
		odd_num.add(LargeInt::Two);
	}
	return odd_num;
}
//生成公钥
LargeInt Rsa::get_public_key(NumLength num_length)
{
	LargeInt test = createPrime(num_length);
	cout << test << endl;
	//return test;
	return 65537;
}

LargeInt Rsa::get_private_key()
{
	LargeInt test = public_key_.extendEuclid(modulus_);
	cout << test << endl;
	
	return test;
}
//公钥加密//TODO:加密的时候要验证明文分组和模数的大小关系
LargeInt Rsa::Encrypt(const LargeInt& plain_text)
{
	/*
	PlainTextList plain_text_list = groupPlainText(plain_text);
	CipherTextList cipher_text_list;
	for (PlainTextList::iterator i = plain_text_list.begin(); i != plain_text_list.end(); ++i)
	{
		cipher_text_list.push_back(i->ModularPower(public_key_, modulus_));
	}
	return mergeCipherTextList(cipher_text_list);
	*/
	return plain_text.ModularPower(public_key_, modulus_);
}
//私钥解密
LargeInt Rsa::Decode(const LargeInt& cipher_text)
{
	return cipher_text.ModularPower(private_key_, modulus_);
}

ostream & operator<<(ostream & out, const Rsa & rsa)
{
	out << "modulus:" << rsa.modulus_ << "\n";
	out << "prime_number1:" << rsa.prime_number1 << "\n";
	out << "prime_number2:" << rsa.prime_number2 << "\n";
	out << "public_key:" << rsa.public_key_ << "\n";
	out << "private_key:" << rsa.private_key_;
	return out;
}