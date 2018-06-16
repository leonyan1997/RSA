#include "stdafx.h"

#include "rsa.h"
#include<iostream>
#include<sstream>
#include<ctime>
#include<cstdlib>

using std::cout;
using std::endl;
using std::ostringstream;

Rsa::Rsa(unsigned int prime_number_length)
{
	srand(time(NULL));
	//����������p��q
	prime_number1 = createPrime(prime_number_length, 10);
	prime_number2 = createPrime(prime_number_length, 10);

	modulus_ = prime_number1 * prime_number2;

	euler_function_num_ = (prime_number1 - 1)*(prime_number2 - 1);

	public_key_ = get_public_key();//TODO:��Կ��Ȼ�ǹ̶��ģ�����
								   //get_public_key(euler_function_num_);
	private_key_ = get_private_key();
}

Rsa::~Rsa()
{
}

void Rsa::Init(unsigned int prime_number_length)
{
	srand(time(NULL));
	//����������p��q
	prime_number1 = createPrime(prime_number_length, 10);
	prime_number2 = createPrime(prime_number_length, 10);

	modulus_ = prime_number1 * prime_number2;

	euler_function_num_ = (prime_number1 - 1)*(prime_number2 - 1);

	public_key_ = get_public_key();//TODO:��Կ��Ȼ�ǹ̶��ģ�����
	//get_public_key(euler_function_num_);
	private_key_ = get_private_key();
}
//���ɳ���Ϊn������
LargeInt Rsa::createOddNum(unsigned int odd_num_length)
{
	odd_num_length /= 4;//��Ϊ��2����ת����16���ƣ�λ��Ҫ��Ϊԭ�ȵ�1/4
	static unsigned char hex_table[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (odd_num_length)
	{
		ostringstream oss;
		for (std::size_t i = 0; i<odd_num_length - 1; ++i)
			oss << hex_table[rand() % 16];
		oss << hex_table[1];
		string str(oss.str());
		return LargeInt(str);
	}
	else
		return LargeInt::Zero;
}
//�ж�����
bool Rsa::isPrime(const LargeInt& n, const unsigned int k)
{
	assert(n != LargeInt::Zero);
	if (n == LargeInt::Two)
		return true;

	LargeInt n_1 = n - 1;
	LargeInt::bit b(n_1);//������λ
	if (b.at(0) == 1)
		return false;

	for (std::size_t t = 0; t<k; ++t)
	{
		LargeInt a = createRandomSmallThan(n_1);//�����
		LargeInt d(LargeInt::One);
		for (int i = b.size() - 1; i >= 0; --i)
		{
			LargeInt x = d;
			d = (d*d) % n;
			if (d == LargeInt::One && x != LargeInt::One && x != n_1)
				return false;

			if (b.at(i))
			{
				assert(d != LargeInt::Zero);
				d = (a*d) % n;
			}
		}
		if (d != LargeInt::One)
			return false;
	}
	return true;
}

LargeInt Rsa::createRandomSmallThan(const LargeInt& a)
{
	unsigned long t = 0;
	do
	{
		t = rand();
	} while (t == 0);

	LargeInt mod(t);
	LargeInt r = mod % a;
	if (r == LargeInt::Zero)
		r = a - LargeInt::One;
	return r;
}
//���ɳ���Ϊn������
LargeInt Rsa::createPrime(unsigned int prime_number_length, int it_count)
{
	assert(it_count>0);
	LargeInt odd_num = createOddNum(prime_number_length);
	while (!isPrime(odd_num, it_count))
	{
		odd_num.add(LargeInt::Two);
	}
	return odd_num;
}
//��һ��ŷ�������������ɹ�Կ��˽Կ
LargeInt Rsa::get_public_key()
{
	return LargeInt(65537);//TODO:��Կ��Ȼ�ǹ̶��ģ�����
}

LargeInt Rsa::get_private_key()
{
	return public_key_.extendEuclid(euler_function_num_);
}
//��Կ����//TODO:���ܵ�ʱ��Ҫ��֤���ķ����ģ���Ĵ�С��ϵ
LargeInt Rsa::Encrypt(const LargeInt& plain_text)
{
	return plain_text.moden(public_key_, modulus_);
}

LargeInt Rsa::Decode(const LargeInt& c)
{//˽Կ����
	return c.moden(private_key_, modulus_);
}
/*
LargeInt Rsa::encryptByPr(const LargeInt& m)
{//˽Կ����
	return decode(m);
}

LargeInt Rsa::decodeByPu(const LargeInt& c)
{//��Կ����
	return encrypt(c);
}
*/