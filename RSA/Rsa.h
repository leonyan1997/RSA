#pragma once
#include"BigInt.h"
class Rsa
{
public:
	Rsa();
	~Rsa();
	void Init(unsigned int n);//初始化，产生公私钥对

public:
	BigInt Encrypt(const BigInt& m);//公钥加密
	BigInt Decode(const BigInt& c);//私钥解密

	//BigInt encryptByPr(const BigInt& m);//公钥加密
	//BigInt decodeByPu(const BigInt& m);//私钥解密
private:
	BigInt createOddNum(unsigned int n);//生成长度为n的奇数
	bool isPrime(const BigInt& a, const unsigned int k);//判断素数
	BigInt createPrime(unsigned int n, int it_cout);//生成长度为n的素数
	BigInt get_public_key();//根据欧拉函数数中生成公钥
	BigInt get_private_key();//根据欧拉函数数和公钥生成私钥
	BigInt createRandomSmallThan(const BigInt& a);//创建小数
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
	BigInt public_key_;//公钥
	BigInt modulus_;//模数
private:
	BigInt private_key_;//私钥
	BigInt prime_number1, prime_number2;//
	BigInt euler_function_num_;//欧拉数
};