#pragma once
#include"BigInt.h"
class Rsa
{
public:
	Rsa();
	~Rsa();
	void Init(unsigned int n);//��ʼ����������˽Կ��

public:
	BigInt Encrypt(const BigInt& m);//��Կ����
	BigInt Decode(const BigInt& c);//˽Կ����

	//BigInt encryptByPr(const BigInt& m);//��Կ����
	//BigInt decodeByPu(const BigInt& m);//˽Կ����
private:
	BigInt createOddNum(unsigned int n);//���ɳ���Ϊn������
	bool isPrime(const BigInt& a, const unsigned int k);//�ж�����
	BigInt createPrime(unsigned int n, int it_cout);//���ɳ���Ϊn������
	BigInt get_public_key();//����ŷ�������������ɹ�Կ
	BigInt get_private_key();//����ŷ���������͹�Կ����˽Կ
	BigInt createRandomSmallThan(const BigInt& a);//����С��
	friend ostream& operator <<(ostream& out, const Rsa& rsa)//���
	{
		out << "N:" << rsa.modulus_ << "\n";
		out << "p:" << rsa.prime_number1 << "\n";
		out << "q:" << rsa.prime_number2 << "\n";
		out << "e:" << rsa.public_key_ << "\n";
		out << "d:" << rsa.private_key_;
		return out;
	}
public:
	BigInt public_key_;//��Կ
	BigInt modulus_;//ģ��
private:
	BigInt private_key_;//˽Կ
	BigInt prime_number1, prime_number2;//
	BigInt euler_function_num_;//ŷ����
};