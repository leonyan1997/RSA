#include "stdafx.h"

#include"large_integer.h"
#include<cassert>

int LargeInt::base_char = 8;
int LargeInt::base = 0xFFFFFFFF;
int LargeInt::basebit = 5;//2^5
int LargeInt::basebitchar = 0x1F;
int LargeInt::basebitnum = 32;
LargeInt LargeInt::Zero(0);
LargeInt LargeInt::One(1);
LargeInt LargeInt::Two(2);

LargeInt operator + (const LargeInt& a, const LargeInt& b)
{
	LargeInt ca(a);
	return ca.add(b);
}

LargeInt operator - (const LargeInt& a, const LargeInt& b)
{
	LargeInt ca(a);
	return ca.sub(b);
}

LargeInt operator * (const LargeInt& a, const LargeInt& b)
{
	if (a == (LargeInt::Zero) || b == (LargeInt::Zero))
		return LargeInt::Zero;

	const LargeInt &big = a.data_.size()>b.data_.size() ? a : b;
	const LargeInt &small = (&big) == (&a) ? b : a;

	LargeInt result(0);

	LargeInt::bit bt(small);
	for (int i = bt.get_size() - 1; i >= 0; --i)
	{
		if (bt.at(i))
		{
			LargeInt temp(big, false);
			temp.leftShift(i);
			//std::cout<<"tmp:"<<temp<<std::endl;
			result.add(temp);
			//std::cout<<"res:"<<result<<std::endl;
		}
	}
	result.is_negative_ = !(a.is_negative_ == b.is_negative_);
	return result;
}

LargeInt operator / (const LargeInt& a, const LargeInt& b)
{
	assert(b != (LargeInt::Zero));
	if (a.equals(b))//绝对值相等
		return (a.is_negative_ == b.is_negative_) ? LargeInt(1) : LargeInt(-1);
	else if (a.smallThan(b))//绝对值小于
		return LargeInt::Zero;
	else
	{
		LargeInt result, ca;
		LargeInt::div(a, b, result, ca);
		return result;
	}
}

LargeInt operator % (const LargeInt& a, const LargeInt& b)
{
	assert(b != (LargeInt::Zero));
	if (a.equals(b))
		return LargeInt::Zero;
	else if (a.smallThan(b))
		return a;
	else
	{
		LargeInt result, ca;
		LargeInt::div(a, b, result, ca);
		return ca;
	}
}

void LargeInt::div(const LargeInt& a, const LargeInt& b, LargeInt& result, LargeInt& ca)
{
	//1.复制a,b
	LargeInt cb(b, false);
	ca.is_negative_ = false;
	ca.data_ = a.data_;

	LargeInt::bit bit_b(cb);
	//位数对齐
	while (true)//绝对值小于
	{
		LargeInt::bit bit_a(ca);
		int len = bit_a.get_size() - bit_b.get_size();
		LargeInt temp;
		//找到移位的
		while (len >= 0)
		{
			temp = cb << len;
			if (temp.smallOrEquals(ca))
				break;
			--len;
		}
		if (len<0)
			break;
		LargeInt::base_t n = 0;
		while (temp.smallOrEquals(ca))
		{
			ca.sub(temp);
			++n;
		}
		LargeInt kk(n);
		if (len)
			kk.leftShift(len);
		result.add(kk);
	}
	result.trim();
}

bool operator < (const LargeInt& a, const LargeInt& b)
{
	if (a.is_negative_ == b.is_negative_)
	{
		if (a.is_negative_ == false)
			return a.smallThan(b);
		else
			return !(a.smallOrEquals(b));
	}
	else
	{
		if (a.is_negative_ == false)
			return true;
		else
			return false;
	}
}

bool operator <= (const LargeInt& a, const LargeInt& b)
{
	if (a.is_negative_ == b.is_negative_)
	{//同号
		if (a.is_negative_ == false)
			return a.smallOrEquals(b);
		else
			return !(a.smallThan(b));
	}
	else//异号
	{
		if (a.is_negative_ == false)
			return true;
		else
			return false;
	}
}

bool operator == (const LargeInt& a, const LargeInt& b)
{
	return a.data_ == b.data_ && a.is_negative_ == b.is_negative_;
}

ostream& operator << (ostream& out, const LargeInt& a)
{
	static char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (a.is_negative_)
		out << "-";
	LargeInt::base_t T = 0x0F;
	string str;
	for (LargeInt::data_t::const_iterator it = a.data_.begin(); it != a.data_.end(); ++it)
	{
		LargeInt::base_t ch = (*it);
		for (int j = 0; j<LargeInt::base_char; ++j)
		{
			str.push_back(hex[ch&(T)]);
			ch = ch >> 4;
		}
	}
	reverse(str.begin(), str.end());
	out << str;
	return out;
}

LargeInt operator <<(const LargeInt& a, unsigned int n)
{
	LargeInt ca(a);
	return ca.leftShift(n);
}

LargeInt& LargeInt::leftShift(const unsigned int n)
{
	int k = n >> (LargeInt::basebit);//5
	int off = n & (LargeInt::basebitchar);//0xFF

	int inc = (off == 0) ? k : 1 + k;
	for (int i = 0; i<inc; ++i)
		data_.push_back(0);

	if (k)
	{
		inc = (off == 0) ? 1 : 2;
		for (int i = data_.size() - inc; i >= k; --i)
			data_[i] = data_[i - k];
		for (int i = 0; i<k; ++i)
			data_[i] = 0;
	}

	if (off)
	{
		LargeInt::base_t T = LargeInt::base;//0xffffffff
		T = T << (LargeInt::basebitnum - off);//32
											//左移
		LargeInt::base_t ch = 0;
		for (uint64_t i = 0; i<data_.size(); ++i)
		{
			LargeInt::base_t t = data_[i];
			data_[i] = (t << off) | ch;
			ch = (t&T) >> (LargeInt::basebitnum - off);//32,最高位
		}
	}
	trim();
	return *this;
}

LargeInt& LargeInt::rightShift(const unsigned int n)
{
	int k = n >> (LargeInt::basebit);//5
	int off = n & (LargeInt::basebitchar);//0xFF

	if (k)
	{
		for (int i = 0; i>k; ++i)
			data_[i] = data_[i + k];
		for (int i = 0; i<k; ++i)
			data_.pop_back();
		if (data_.size() == 0)
			data_.push_back(0);
	}

	if (off)
	{
		LargeInt::base_t T = LargeInt::base;//0xFFFFFFFF
		T = T >> (LargeInt::basebitnum - off);//32
											//左移
		LargeInt::base_t ch = 0;
		for (int i = data_.size() - 1; i >= 0; --i)
		{
			LargeInt::base_t t = data_[i];
			data_[i] = (t >> off) | ch;
			ch = (t&T) << (LargeInt::basebitnum - off);//32,最高位
		}
	}
	trim();
	return *this;
}

LargeInt& LargeInt::sub(const LargeInt& b)
{
	if (b.is_negative_ == is_negative_)
	{//同号

		LargeInt::data_t &res = data_;
		if (!(smallThan(b)))//绝对值大于b
		{
			int cn = 0;//借位
					   //大数减小数
			for (uint64_t i = 0; i<b.data_.size(); ++i)
			{
				LargeInt::base_t temp = res[i];
				res[i] = (res[i] - b.data_[i] - cn);
				cn = temp<res[i] ? 1 : temp<b.data_[i] ? 1 : 0;
			}

			for (uint64_t i = b.data_.size(); i<data_.size() && cn != 0; ++i)
			{
				LargeInt::base_t temp = res[i];
				res[i] = res[i] - cn;
				cn = temp<cn;
			}
			trim();
		}
		else//绝对值小于b
		{
			data_ = (b - (*this)).data_;
			is_negative_ = !is_negative_;
		}
	}
	else
	{//异号的情况
		bool isnegative = is_negative_;
		is_negative_ = b.is_negative_;
		add(b);
		is_negative_ = isnegative;
	}
	return *this;
}
//将数据拆分成32位为1组逆向存放，并含有真实数据的前导零
void LargeInt::copyFromHexString(const string & s)
{
	string str(s);
	//如果str是负的，则符号位置零，并删去符号
	if (str.length() && str.at(0) == '-')
	{
		if (str.length()>1)
			is_negative_ = true;
		str = str.substr(1);
	}

	//将str的位数用前导0补齐至8的倍数
	int count = (8 - (str.length() % 8)) % 8;
	std::string temp;
	for (int i = 0; i<count; ++i)
		temp.push_back(0);
	str = temp + str;

	//每8字节的处理
	for (int i = 0; i<str.length(); i += LargeInt::base_char)
	{
		base_t sum = 0;
		//对每字节开始处理
		for (int j = 0; j<base_char; ++j)
		{
			//i+j代表实际位数
			char ch = str[i + j];

			//1位16进制字符变成4位2进制数
			ch = hexCharToBin(ch);
			sum = ((sum << 4) | (ch));
		}
		data_.push_back(sum);
	}
	reverse(data_.begin(), data_.end());
}

//将1位16进制字符变成4位2进制数
char LargeInt::hexCharToBin(char ch)
{
	static char table[] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f };
	if (isdigit(ch))
		ch -= '0';
	else if (islower(ch))
		ch -= 'a' - 10;
	else if (isupper(ch))
		ch -= 'A' - 10;

	return table[ch];
}
//将数据拆分成32位为1组逆向存放
void LargeInt::copyFromLong(const long n)
{
	long a = n;
	if (a<0)
	{
		is_negative_ = true;
		a = -a;
	}
	do
	{
		//截取最后的32位
		LargeInt::base_t ch = (a&(LargeInt::base));
		data_.push_back(ch);
		a = a >> (LargeInt::basebitnum);
	} while (a);
}

LargeInt& LargeInt::add(const LargeInt& b)
{
	if (is_negative_ == b.is_negative_)
	{//同号
	 //引用
		LargeInt::data_t &res = data_;
		int len = b.data_.size() - data_.size();

		while (len--)//高位补0
			res.push_back(0);

		int cn = 0;//进位
		for (uint64_t i = 0; i<b.data_.size(); ++i)
		{
			LargeInt::base_t temp = res[i];
			res[i] = res[i] + b.data_[i] + cn;
			cn = temp>res[i] ? 1 : temp>(temp + b.data_[i]) ? 1 : 0;//0xFFFFFFFF
		}

		for (uint64_t i = b.data_.size(); i<data_.size() && cn != 0; ++i)
		{
			LargeInt::base_t temp = res[i];
			res[i] = (res[i] + cn);
			cn = temp>res[i];
		}

		if (cn != 0)
			res.push_back(cn);

		trim();
	}
	else
	{//异号的情况
		bool isnegative;
		if (smallThan(b))//绝对值小于b
			isnegative = b.is_negative_;
		else if (equals(b))//绝对值等于b
			isnegative = false;
		else//绝对值大于b
			isnegative = is_negative_;

		is_negative_ = b.is_negative_;
		sub(b);
		is_negative_ = isnegative;
	}
	return *this;
}
//蒙哥马利幂模运算
LargeInt LargeInt::moden(const LargeInt& public_key, const LargeInt& modulus)const
{
	LargeInt::bit t(public_key);

	LargeInt cipher_text(1);
	for (int i = t.get_size() - 1; i >= 0; --i)
	{
		cipher_text = (cipher_text*cipher_text) % modulus;
		if (t.at(i))
			cipher_text = (cipher_text*(*this)) % modulus;
	}
	return cipher_text;
}

LargeInt LargeInt::extendEuclid(const LargeInt& modulus)
{//扩展欧几里得算法求乘法逆元
	assert(modulus.is_negative_ == false);//modulus为正数
	LargeInt a[3], b[3], t[3];
	a[0] = 1; a[1] = 0; a[2] = modulus;
	b[0] = 0; b[1] = 1; b[2] = *this;
	if (b[2] == LargeInt::Zero || b[2] == LargeInt::One)
		return b[2];

	while (true)
	{
		if (b[2] == LargeInt::One)
		{
			if (b[1].is_negative_ == true)//负数
				b[1] = (b[1] % modulus + modulus) % modulus;
			return b[1];
		}

		LargeInt q = a[2] / b[2];
		for (int i = 0; i<3; ++i)
		{
			t[i] = a[i] - q * b[i];
			a[i] = b[i];
			b[i] = t[i];
		}
	}
}

LargeInt::LargeInt() :is_negative_(false)
{
	data_.push_back(0);
}
//每32位1组，逆向存放
LargeInt::LargeInt(const string & num) : data_(), is_negative_(false)
{
	copyFromHexString(num); 
	trim();
}
//每32位1组，逆向存放
LargeInt::LargeInt(const long n) : is_negative_(false)
{
	copyFromLong(n);
}

LargeInt::LargeInt(const_data_t data) : data_(data), is_negative_(false) 
{
	trim();
}


LargeInt & LargeInt::operator=(string s)
{
	data_.clear();
	is_negative_ = false;
	copyFromHexString(s);
	trim();
	return *this;
}

LargeInt::LargeInt(const LargeInt & a, bool isnegative) :data_(a.data_), is_negative_(isnegative) {}

LargeInt & LargeInt::operator=(const long n)
{
	data_.clear();
	copyFromLong(n);
	return *this;
}

uint64_t LargeInt::bit::get_size()
{
	return size_;
}

bool LargeInt::bit::at(uint64_t i)
{
	uint64_t index = i >> (LargeInt::basebit);
	uint64_t off = i & (LargeInt::basebitchar);
	LargeInt::base_t t = data_[index];
	return (t&(1 << off));
}

LargeInt::bit::bit(const LargeInt& ba)
{
	data_ = ba.data_;
	LargeInt::base_t a = data_[data_.size() - 1];//最高位
	size_ = data_.size() << (LargeInt::basebit);
	LargeInt::base_t t = 1 << (LargeInt::basebitnum - 1);

	if (a == 0)
		size_ -= (LargeInt::basebitnum);
	else
	{
		while (!(a&t))
		{
			--size_;
			t = t >> 1;
		}
	}
}

bool LargeInt::smallThan(const LargeInt& b)const
{
	if (data_.size() == b.data_.size())
	{
		for (LargeInt::data_t::const_reverse_iterator it = data_.rbegin(), it_b = b.data_.rbegin();it != data_.rend(); ++it, ++it_b)
			if ((*it) != (*it_b))
				return (*it)<(*it_b);
		return false;//相等
	}
	else
		return data_.size()<b.data_.size();
}

bool LargeInt::smallOrEquals(const LargeInt& b)const
{
	if (data_.size() == b.data_.size())
	{
		for (LargeInt::data_t::const_reverse_iterator it = data_.rbegin(), it_b = b.data_.rbegin();
			it != data_.rend(); ++it, ++it_b)
			if ((*it) != (*it_b))
				return (*it)<(*it_b);
		return true;//相等
	}
	else
		return data_.size()<b.data_.size();
}

bool LargeInt::equals(const LargeInt& a)const
{
	return data_ == a.data_;
}
//因为数据是逆向存放的，所以是消除实际数据的前导零。
LargeInt & LargeInt::trim()
{
	int count = 0;
	//检查不为0的元素的数量		
	for (data_t::reverse_iterator it = data_.rbegin(); it != data_.rend(); ++it)
		if ((*it) == 0)
			++count;
		else
			break;
	if (count == data_.size())//只有零的情况保留			
		--count;
	for (int i = 0; i<count; ++i)
		data_.pop_back();
	return *this;
}
