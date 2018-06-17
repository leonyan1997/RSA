#pragma once
#include<vector>
#include<string>
#include<cassert>
#include<iostream>
#include <algorithm>
#include <cstdint>
using std::ostream;
using std::vector;
using std::string;

typedef uint32_t base_t;
typedef vector<base_t> data_t;
typedef const vector<base_t> const_data_t;

class LargeInt {

public:
	static int base_num_length;
	static int base;
	static int base_bit_num;
	static int basebitchar;
	static int base_bit_length;
private:
	friend class Rsa;
public:
	friend LargeInt operator + (const LargeInt& a, const LargeInt& b);
	friend LargeInt operator - (const LargeInt& a, const LargeInt& b);
	friend LargeInt operator * (const LargeInt& a, const LargeInt& b);
	friend LargeInt operator / (const LargeInt& a, const LargeInt& b);
	friend LargeInt operator % (const LargeInt& a, const LargeInt& b);
	friend bool operator < (const LargeInt& a, const LargeInt& b);
	friend bool operator <= (const LargeInt& a, const LargeInt& b);
	friend bool operator == (const LargeInt& a, const LargeInt& b);
	friend bool operator != (const LargeInt& a, const LargeInt& b) { return !(a == b); }
	//���ذ汾	
	friend LargeInt operator + (const LargeInt& a, const long b) { LargeInt t(b); return a + t; }
	friend LargeInt operator - (const LargeInt& a, const long b) { LargeInt t(b); return a - t; }
	friend LargeInt operator * (const LargeInt& a, const long b) { LargeInt t(b); return a * t; }
	friend LargeInt operator / (const LargeInt& a, const long b) { LargeInt t(b); return a / t; }
	friend LargeInt operator % (const LargeInt& a, const long b) { LargeInt t(b); return a % t; }
	friend bool operator < (const LargeInt& a, const long b) { LargeInt t(b); return a < t; }
	friend bool operator <= (const LargeInt& a, const  long b) { LargeInt t(b); return a <= t; }
	friend bool operator == (const LargeInt& a, const long b) { LargeInt t(b); return a == t; }
	friend bool operator != (const LargeInt& a, const long b) { LargeInt t(b); return !(a == t); };
	//	
	friend ostream& operator << (ostream& out, const LargeInt& a);
	friend LargeInt operator <<(const LargeInt& a, unsigned int n);
public:
	LargeInt& trim();//ɾ��ǰ����

	friend class bit;
	class bit
	{
	public:
		uint64_t get_size();
		bool at(uint64_t i);
		bit(const LargeInt& a);
	private:
		vector<base_t> data_;
		uint64_t bit_size_;
	};
	//������ģ����	
	LargeInt ModularPower(const LargeInt& exp, const LargeInt& p)const;
	/* ����չ��ŷ������㷨��˷���Ԫ */
	LargeInt extendEuclid(const LargeInt& m);
public:
	LargeInt();
	LargeInt(const string& num);
	LargeInt(const long n);
	LargeInt(const_data_t data);
	LargeInt(const LargeInt& a, bool isnegative);
	LargeInt& operator =(string s);
	LargeInt& operator =(const long n);

public:
	static LargeInt Zero;
	static LargeInt One;
	static LargeInt Two;
private:
	bool smallThan(const LargeInt& a)const;//�жϾ���ֵ�Ƿ�С��
	bool smallOrEquals(const LargeInt& a)const;//�жϾ���ֵ�Ƿ�С�����
	bool equals(const LargeInt& a)const;//�жϾ���ֵ�Ƿ����

	LargeInt& leftShift(const unsigned int n);
	LargeInt& rightShift(const unsigned int n);
	LargeInt& add(const LargeInt& b);
	LargeInt& sub(const LargeInt& b);
	void copyFromHexString(const string& s);
	char hexCharToBin(char ch);
	void copyFromLong(const long n);
	static void div(const LargeInt& a, const LargeInt& b, LargeInt& result, LargeInt& ca);
private:
	vector<base_t> data_;//���ݴ洢	
	
	bool is_negative_;//�Ƿ�Ϊ����
};
