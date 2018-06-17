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
/*
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
}*/
//���ɳ���Ϊtest_num������
LargeInt Rsa::createOddNum(unsigned int odd_num_length)
{
	odd_num_length /= 4;//��Ϊ��2����ת����16���ƣ�λ��Ҫ��Ϊԭ�ȵ�1/4
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
//�ж��������������
bool Rsa::isPrime(const LargeInt& test_num, const unsigned int times_couter)
{
	if (test_num == LargeInt::Two)
		return true;

	LargeInt num_1 = test_num - 1;
	LargeInt::bit bit_num_1(num_1);//�����Ƶ�num_1
	if (bit_num_1.at(0) == 1)//���bitλ��1��˵������������δ��1ǰ����ż�����Ҳ���2�����Է���false��
		return false;

	for (uint64_t t = 0; t<times_couter; ++t)
	{
		LargeInt random_num = createRandomSmallThan(num_1);//�����
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

PlainTextList Rsa::groupPlainText(const PlainText &)
{
	return PlainTextList();
}

CipherText Rsa::mergeCipherTextList(const CipherTextList &)
{
	return CipherText();
}
//���ɳ���Ϊtest_num������
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
	return public_key_.extendEuclid(modulus_);
}
//��Կ����//TODO:���ܵ�ʱ��Ҫ��֤���ķ����ģ���Ĵ�С��ϵ
LargeInt Rsa::Encrypt(const LargeInt& plain_text)
{
	/*
	PlainTextList plain_text_list = groupPlainText(plain_text);
	CipherTextList cipher_text_list;
	for (PlainTextList::iterator i = plain_text_list.begin(); i != plain_text_list.end(); ++i)
	{
		cipher_text_list.push_back(i->moden(public_key_, modulus_));
	}
	return mergeCipherTextList(cipher_text_list);
	*/
	return plain_text.moden(public_key_, modulus_);
}
//˽Կ����
LargeInt Rsa::Decode(const LargeInt& cipher_text)
{
	return cipher_text.moden(private_key_, modulus_);
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