/*
该文件是Rsa系统类的声明。
*/




#pragma once
#include"large_integer.h"

typedef vector<LargeInt> PlainTextList;
typedef vector<LargeInt> CipherTextList;
typedef LargeInt PlainText;
typedef LargeInt CipherText;
typedef uint32_t NumLength;
typedef uint64_t Num;

class Rsa
{
	friend ostream& operator <<(ostream& out, const Rsa& rsa);//输出
public:
	Rsa(NumLength prime_number_length);//初始化，产生公私钥对
	~Rsa();

public:
	LargeInt Encrypt(const LargeInt& plain_text);//公钥加密
	LargeInt Decode(const LargeInt& cipher_text);//私钥解密
private:
	LargeInt createOddNum(NumLength length);//生成长度为n的奇数
	bool isPrime(const LargeInt& test_num);//米勒拉宾测试判断素数
	LargeInt createPrime(NumLength prime_number_length);//生成长度为n的素数
	LargeInt get_public_key(NumLength num_length);//根据欧拉函数数生成公钥
	LargeInt get_private_key();//根据欧拉函数数和公钥生成私钥
	LargeInt createRandomSmallThan(const LargeInt & value);//创建比value小的随机数

public:
	LargeInt public_key_;//公钥
	LargeInt modulus_;//模数
private:
	LargeInt private_key_;//私钥
	LargeInt prime_number1, prime_number2;//两个素数因子
	LargeInt euler_function_num_;//欧拉数

								 
//未实现的函数
private:
	PlainTextList groupPlainText(PlainText plain_text);//将长明文分组
	CipherText mergeCipherTextList(const CipherTextList&);//将密文分组合并
};