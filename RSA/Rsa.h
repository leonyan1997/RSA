/*
���ļ���Rsaϵͳ���������
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
	friend ostream& operator <<(ostream& out, const Rsa& rsa);//���
public:
	Rsa(NumLength prime_number_length);//��ʼ����������˽Կ��
	~Rsa();

public:
	LargeInt Encrypt(const LargeInt& plain_text);//��Կ����
	LargeInt Decode(const LargeInt& cipher_text);//˽Կ����
private:
	LargeInt createOddNum(NumLength length);//���ɳ���Ϊn������
	bool isPrime(const LargeInt& test_num);//�������������ж�����
	LargeInt createPrime(NumLength prime_number_length);//���ɳ���Ϊn������
	LargeInt get_public_key(NumLength num_length);//����ŷ�����������ɹ�Կ
	LargeInt get_private_key();//����ŷ���������͹�Կ����˽Կ
	LargeInt createRandomSmallThan(const LargeInt & value);//������valueС�������

public:
	LargeInt public_key_;//��Կ
	LargeInt modulus_;//ģ��
private:
	LargeInt private_key_;//˽Կ
	LargeInt prime_number1, prime_number2;//������������
	LargeInt euler_function_num_;//ŷ����

								 
//δʵ�ֵĺ���
private:
	PlainTextList groupPlainText(PlainText plain_text);//�������ķ���
	CipherText mergeCipherTextList(const CipherTextList&);//�����ķ���ϲ�
};