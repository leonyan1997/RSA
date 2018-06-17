#pragma once
#include"large_integer.h"

typedef vector<LargeInt> PlainTextList;
typedef vector<LargeInt> CipherTextList;
typedef LargeInt PlainText;
typedef LargeInt CipherText;

class Rsa
{
	friend ostream& operator <<(ostream& out, const Rsa& rsa);//输出
public:
	Rsa(unsigned int prime_number_length);
	~Rsa();
	//void Init(unsigned int n);//初始化，产生公私钥对


public:
	LargeInt Encrypt(const LargeInt& m);//公钥加密
	LargeInt Decode(const LargeInt& c);//私钥解密
private:
	LargeInt createOddNum(unsigned int n);//生成长度为n的奇数
	bool isPrime(const LargeInt& a, const unsigned int k);//判断素数
	LargeInt createPrime(unsigned int n, int it_cout);//生成长度为n的素数
	LargeInt get_public_key();//根据欧拉函数数中生成公钥
	LargeInt get_private_key();//根据欧拉函数数和公钥生成私钥
	LargeInt createRandomSmallThan(const LargeInt& a);//创建小数
	PlainTextList groupPlainText(const PlainText&);
	CipherText mergeCipherTextList(const CipherTextList&);

public:
	LargeInt public_key_;//公钥
	LargeInt modulus_;//模数
private:
	LargeInt private_key_;//私钥
	LargeInt prime_number1, prime_number2;//
	LargeInt euler_function_num_;//欧拉数
};