#include "stdafx.h"

#include"large_integer.h"
#include<cassert>

int LargeInt::base_num_length = 8;//1��data����8��16��������
int LargeInt::base = 0xFFFFFFFF;
int LargeInt::base_bit_length = 5;//1��data��ռ��bitλ��������Ӧ����32����Ϊ�����Ʋ���������������5��
int LargeInt::basebitchar = 0x1F;//31
int LargeInt::base_bit_num = 32;//1��data��ռ��bitλ����
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
			//std::cout<<"tmp:"<<zeros<<std::endl;
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
	if (a.equals(b))//����ֵ���
		return (a.is_negative_ == b.is_negative_) ? LargeInt(1) : LargeInt(-1);
	else if (a.smallThan(b))//����ֵС��
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
	//1.����biggest_data,b
	LargeInt cb(b, false);
	ca.is_negative_ = false;
	ca.data_ = a.data_;

	LargeInt::bit bit_b(cb);
	//λ������
	while (true)//����ֵС��
	{
		LargeInt::bit bit_a(ca);
		int len = bit_a.get_size() - bit_b.get_size();
		LargeInt temp;
		//�ҵ���λ��
		while (len >= 0)
		{
			temp = cb << len;
			if (temp.smallOrEquals(ca))
				break;
			--len;
		}
		if (len<0)
			break;
		base_t n = 0;
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
	{//ͬ��
		if (a.is_negative_ == false)
			return a.smallOrEquals(b);
		else
			return !(a.smallThan(b));
	}
	else//���
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
	base_t T = 0x0F;
	string str;
	for (vector<base_t>::const_iterator it = a.data_.begin(); it != a.data_.end(); ++it)
	{
		base_t ch = (*it);
		for (int j = 0; j<LargeInt::base_num_length; ++j)
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
	int k = n >> (LargeInt::base_bit_length);//5
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
		base_t T = LargeInt::base;//0xffffffff
		T = T << (LargeInt::base_bit_num - off);//32
											//����
		base_t ch = 0;
		for (uint64_t i = 0; i<data_.size(); ++i)
		{
			base_t t = data_[i];
			data_[i] = (t << off) | ch;
			ch = (t&T) >> (LargeInt::base_bit_num - off);//32,���λ
		}
	}
	trim();
	return *this;
}

LargeInt& LargeInt::rightShift(const unsigned int n)
{
	int k = n >> (LargeInt::base_bit_length);//5
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
		base_t T = LargeInt::base;//0xFFFFFFFF
		T = T >> (LargeInt::base_bit_num - off);//32
											//����
		base_t ch = 0;
		for (int i = data_.size() - 1; i >= 0; --i)
		{
			base_t t = data_[i];
			data_[i] = (t >> off) | ch;
			ch = (t&T) << (LargeInt::base_bit_num - off);//32,���λ
		}
	}
	trim();
	return *this;
}

LargeInt& LargeInt::sub(const LargeInt& b)
{
	if (b.is_negative_ == is_negative_)
	{//ͬ��

		data_t &res = data_;
		if (!(smallThan(b)))//����ֵ����b
		{
			int cn = 0;//��λ
					   //������С��
			for (uint64_t i = 0; i<b.data_.size(); ++i)
			{
				base_t temp = res[i];
				res[i] = (res[i] - b.data_[i] - cn);
				cn = temp<res[i] ? 1 : temp<b.data_[i] ? 1 : 0;
			}

			for (uint64_t i = b.data_.size(); i<data_.size() && cn != 0; ++i)
			{
				base_t temp = res[i];
				res[i] = res[i] - cn;
				cn = temp<cn;
			}
			trim();
		}
		else//����ֵС��b
		{
			data_ = (b - (*this)).data_;
			is_negative_ = !is_negative_;
		}
	}
	else
	{//��ŵ����
		bool isnegative = is_negative_;
		is_negative_ = b.is_negative_;
		add(b);
		is_negative_ = isnegative;
	}
	return *this;
}
//�����ݲ�ֳ�32λΪ1�������ţ���������ʵ���ݵ�ǰ����
void LargeInt::copyFromHexString(const string & s)
{
	string input_str(s);
	//���input_str�Ǹ��ģ������λ���㣬��ɾȥ����
	if (input_str.length() && input_str.at(0) == '-')
	{
		if (input_str.length()>1)
			is_negative_ = true;
		input_str = input_str.substr(1);
	}

	//��input_str��λ����ǰ��0������LargeInt::base_num_length����8�ı���
	int count = LargeInt::base_num_length - (input_str.length() % LargeInt::base_num_length);
	string zeros;
	for (int i = 0; i<count; ++i)
		zeros.push_back(0);
	input_str = zeros + input_str;

	//ÿ8�ֽڵĴ���
	for (int i = 0; i<input_str.length(); i += LargeInt::base_num_length)
	{
		base_t sum = 0;
		//��ÿ8�ֽڿ�ʼ����
		for (int j = 0; j<LargeInt::base_num_length; ++j)
		{
			//i+j����ʵ��λ��
			//ÿ���ֽڴ洢��1��16�����ַ���ɶ�Ӧ��4λ2������
			//��������ֽ��д�ŵ�16�����ַ��ͱ�ת��Ϊ2���ƣ����δ�Ž�data_�С�һ����8���ַ���
			sum = ((sum << 4) | hexCharToBin(input_str[i + j]));
		}
		data_.push_back(sum);
	}
	//������
	reverse(data_.begin(), data_.end());
}

//��1��6�����ַ����4λ2������
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
//�����ݲ�ֳ�32λΪ1��������
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
		//��ȡ����32λ
		base_t ch = (a&(LargeInt::base));
		data_.push_back(ch);
		a = a >> (LargeInt::base_bit_num);
	} while (a);
}

LargeInt& LargeInt::add(const LargeInt& b)
{
	if (is_negative_ == b.is_negative_)
	{//ͬ��
	 //����
		vector<base_t> &res = data_;
		int len = b.data_.size() - data_.size();

		while (len--)//��λ��0
			res.push_back(0);

		int cn = 0;//��λ
		for (uint64_t i = 0; i<b.data_.size(); ++i)
		{
			base_t temp = res[i];
			res[i] = res[i] + b.data_[i] + cn;
			cn = temp>res[i] ? 1 : temp>(temp + b.data_[i]) ? 1 : 0;//0xFFFFFFFF
		}

		for (uint64_t i = b.data_.size(); i<data_.size() && cn != 0; ++i)
		{
			base_t temp = res[i];
			res[i] = (res[i] + cn);
			cn = temp>res[i];
		}

		if (cn != 0)
			res.push_back(cn);

		trim();
	}
	else
	{//��ŵ����
		bool isnegative;
		if (smallThan(b))//����ֵС��b
			isnegative = b.is_negative_;
		else if (equals(b))//����ֵ����b
			isnegative = false;
		else//����ֵ����b
			isnegative = is_negative_;

		is_negative_ = b.is_negative_;
		sub(b);
		is_negative_ = isnegative;
	}
	return *this;
}
//�ɸ�������ģ����
LargeInt LargeInt::ModularPower(const LargeInt& ex, const LargeInt& modulus)const
{
	LargeInt::bit t(ex);

	LargeInt text(1);
	for (int i = t.get_size() - 1; i >= 0; --i)
	{
		text = (text*text) % modulus;
		if (t.at(i))
			text = (text*(*this)) % modulus;
	}
	return text;
}
//��չŷ������㷨��˷���Ԫ
LargeInt LargeInt::extendEuclid(const LargeInt& modulus)
{
	LargeInt a[3], b[3], t[3];
	a[0] = 1; a[1] = 0; a[2] = modulus;
	b[0] = 0; b[1] = 1; b[2] = *this;
	if (b[2] == LargeInt::Zero || b[2] == LargeInt::One)
		return b[2];

	while (true)
	{
		if (b[2] == LargeInt::One)
		{
			if (b[1].is_negative_ == true)//����
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
//ÿ32λ1�飬������
LargeInt::LargeInt(const string & num) : data_(), is_negative_(false)
{
	copyFromHexString(num); 
	trim();
}
//ÿ32λ1�飬������
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
	return bit_size_;
}

//i����bit��λ��
bool LargeInt::bit::at(uint64_t i)
{
	uint64_t index = i >> (LargeInt::base_bit_length);//�����data��λ��
	uint64_t off = i & (LargeInt::basebitchar);//��ȡ��5λbit,�����ڸ�bit�ڸ�data�е�ƫ����
	base_t t = data_[index];
	return (t&(1 << off));//����׼ȷ��bitλ��
}

LargeInt::bit::bit(const LargeInt& large_int)
{
	//��data_��ֵ
	data_ = large_int.data_;

	//��bit_size_��ֵ
	base_t biggest_data = data_[data_.size() - 1];//���λ������8��16������
	bit_size_ = data_.size() << (LargeInt::base_bit_length);//*2^5�����е�bitλ��
	base_t this_bit_is_1 = 1 << (LargeInt::base_bit_num - 1);//��ʼ��1��data��λ��bit�ε����bitλ��1���������0��

	if (biggest_data == 0)
		bit_size_ -= (LargeInt::base_bit_num);//bit_size_����1��data��bitλ��
	else
	{
		//������ǰ��ʵ�������ϱ�ʾǰ�����bit��λ����
		while (!(biggest_data & this_bit_is_1))
		{
			--bit_size_;
			this_bit_is_1 = this_bit_is_1 >> 1;
		}
	}
}

bool LargeInt::smallThan(const LargeInt& b)const
{
	if (data_.size() == b.data_.size())
	{
		for (vector<base_t>::const_reverse_iterator it = data_.rbegin(), it_b = b.data_.rbegin();it != data_.rend(); ++it, ++it_b)
			if ((*it) != (*it_b))
				return (*it)<(*it_b);
		return false;//���
	}
	else
		return data_.size()<b.data_.size();
}

bool LargeInt::smallOrEquals(const LargeInt& b)const
{
	if (data_.size() == b.data_.size())
	{
		for (vector<base_t>::const_reverse_iterator it = data_.rbegin(), it_b = b.data_.rbegin();
			it != data_.rend(); ++it, ++it_b)
			if ((*it) != (*it_b))
				return (*it)<(*it_b);
		return true;//���
	}
	else
		return data_.size()<b.data_.size();
}

bool LargeInt::equals(const LargeInt& a)const
{
	return data_ == a.data_;
}
//��Ϊ�����������ŵģ�����������ʵ�����ݵ�ǰ���㡣
LargeInt & LargeInt::trim()
{
	int count = 0;
	//��鲻Ϊ0��Ԫ�ص�����		
	for (data_t::reverse_iterator it = data_.rbegin(); it != data_.rend(); ++it)
		if ((*it) == 0)
			++count;
		else
			break;
	if (count == data_.size())//ֻ������������			
		--count;
	for (int i = 0; i<count; ++i)
		data_.pop_back();
	return *this;
}
