#include "stdafx.h"

#include "Rsa.h"
#include<iostream>
#include<sstream>
#include<ctime>
#include<cstdlib>

using std::cout;
using std::endl;
using std::ostringstream;

Rsa::Rsa()
{
}

Rsa::~Rsa()
{
}

void Rsa::Init(unsigned int n)
{
	srand(time(NULL));
	//产生大素数p、q
	prime_number1 = createPrime(n, 10);
	prime_number2 = createPrime(n, 10);

	modulus_ = prime_number1 * prime_number2;

	euler_function_num_ = (prime_number1 - 1)*(prime_number2 - 1);

	public_key_ = get_public_key();//TODO:公钥居然是固定的？！！
	//get_public_key(euler_function_num_);
	private_key_ = get_private_key();
}
//生成长度为n的奇数
BigInt Rsa::createOddNum(unsigned int odd_num_length)
{
	odd_num_length /= 4;//因为从2进制转化到16进制，位数要变为原先的1/4
	static unsigned char hex_table[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (odd_num_length)
	{
		ostringstream oss;
		for (std::size_t i = 0; i<odd_num_length - 1; ++i)
			oss << hex_table[rand() % 16];
		oss << hex_table[1];
		string str(oss.str());
		return BigInt(str);
	}
	else
		return BigInt::Zero;
}
//判断素数
bool Rsa::isPrime(const BigInt& n, const unsigned int k)
{
	assert(n != BigInt::Zero);
	if (n == BigInt::Two)
		return true;

	BigInt n_1 = n - 1;
	BigInt::bit b(n_1);//二进制位
	if (b.at(0) == 1)
		return false;

	for (std::size_t t = 0; t<k; ++t)
	{
		BigInt a = createRandomSmallThan(n_1);//随机数
		BigInt d(BigInt::One);
		for (int i = b.size() - 1; i >= 0; --i)
		{
			BigInt x = d;
			d = (d*d) % n;
			if (d == BigInt::One && x != BigInt::One && x != n_1)
				return false;

			if (b.at(i))
			{
				assert(d != BigInt::Zero);
				d = (a*d) % n;
			}
		}
		if (d != BigInt::One)
			return false;
	}
	return true;
}

BigInt Rsa::createRandomSmallThan(const BigInt& a)
{
	unsigned long t = 0;
	do
	{
		t = rand();
	} while (t == 0);

	BigInt mod(t);
	BigInt r = mod % a;
	if (r == BigInt::Zero)
		r = a - BigInt::One;
	return r;
}
//生成长度为n的素数
BigInt Rsa::createPrime(unsigned int prime_number_length, int it_count)
{
	assert(it_count>0);
	BigInt odd_num = createOddNum(prime_number_length);
	while (!isPrime(odd_num, it_count))
	{
		odd_num.add(BigInt::Two);
	}
	return odd_num;
}
//从一个欧拉函数数中生成公钥、私钥
BigInt Rsa::get_public_key()
{
	return BigInt(65537);//TODO:公钥居然是固定的？！！
}

BigInt Rsa::get_private_key()
{
	return public_key_.extendEuclid(euler_function_num_);
}

BigInt Rsa::Encrypt(const BigInt& m)
{//公钥加密
	return m.moden(public_key_, modulus_);
}

BigInt Rsa::Decode(const BigInt& c)
{//私钥解密
	return c.moden(private_key_, modulus_);
}
/*
BigInt Rsa::encryptByPr(const BigInt& m)
{//私钥加密
	return Decode(m);
}

BigInt Rsa::decodeByPu(const BigInt& c)
{//公钥解密
	return Encrypt(c);
}
*/