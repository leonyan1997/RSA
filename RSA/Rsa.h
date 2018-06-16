#pragma once
#include"large_integer.h"
class Rsa
{
public:
	Rsa(unsigned int prime_number_length);
	~Rsa();
	void Init(unsigned int n);//初始化，产生公私钥对

public:
	LargeInt Encrypt(const LargeInt& m);//公钥加密
	LargeInt Decode(const LargeInt& c);//私钥解密

	//LargeInt encryptByPr(const LargeInt& m);//公钥加密
	//LargeInt decodeByPu(const LargeInt& m);//私钥解密
private:
	LargeInt createOddNum(unsigned int n);//生成长度为n的奇数
	bool isPrime(const LargeInt& a, const unsigned int k);//判断素数
	LargeInt createPrime(unsigned int n, int it_cout);//生成长度为n的素数
	LargeInt get_public_key();//根据欧拉函数数中生成公钥
	LargeInt get_private_key();//根据欧拉函数数和公钥生成私钥
	LargeInt createRandomSmallThan(const LargeInt& a);//创建小数
	friend ostream& operator <<(ostream& out, const Rsa& rsa)//输出
	{
		out << "N:" << rsa.modulus_ << "\n";
		out << "p:" << rsa.prime_number1 << "\n";
		out << "q:" << rsa.prime_number2 << "\n";
		out << "e:" << rsa.public_key_ << "\n";
		out << "d:" << rsa.private_key_;
		return out;
	}
public:
	LargeInt public_key_;//公钥
	LargeInt modulus_;//模数
private:
	LargeInt private_key_;//私钥
	LargeInt prime_number1, prime_number2;//
	LargeInt euler_function_num_;//欧拉数
};