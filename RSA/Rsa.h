#pragma once
#include"large_integer.h"

typedef vector<LargeInt> PlainTextList;
typedef vector<LargeInt> CipherTextList;
typedef LargeInt PlainText;
typedef LargeInt CipherText;

class Rsa
{
	friend ostream& operator <<(ostream& out, const Rsa& rsa);//���
public:
	Rsa(unsigned int prime_number_length);
	~Rsa();
	//void Init(unsigned int n);//��ʼ����������˽Կ��


public:
	LargeInt Encrypt(const LargeInt& m);//��Կ����
	LargeInt Decode(const LargeInt& c);//˽Կ����
private:
	LargeInt createOddNum(unsigned int n);//���ɳ���Ϊn������
	bool isPrime(const LargeInt& a, const unsigned int k);//�ж�����
	LargeInt createPrime(unsigned int n, int it_cout);//���ɳ���Ϊn������
	LargeInt get_public_key();//����ŷ�������������ɹ�Կ
	LargeInt get_private_key();//����ŷ���������͹�Կ����˽Կ
	LargeInt createRandomSmallThan(const LargeInt& a);//����С��
	PlainTextList groupPlainText(const PlainText&);
	CipherText mergeCipherTextList(const CipherTextList&);

public:
	LargeInt public_key_;//��Կ
	LargeInt modulus_;//ģ��
private:
	LargeInt private_key_;//˽Կ
	LargeInt prime_number1, prime_number2;//
	LargeInt euler_function_num_;//ŷ����
};