#include "stdafx.h"

#include "rsa_system.h"



//�˵���ʾ����
void RsaSystem::menu()
{
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "               e.encrypt ����              " << endl;
	cout << "               d.decrypt ����              " << endl;
	cout << "               s.setkey ����               " << endl;
	cout << "               p.print ��ʾ               " << endl;
	cout << "               q.quit �˳�                 " << endl;
	cout << "input your choice:" << endl;
}

bool RsaSystem::isLegal(const string& str)
{//�ж������Ƿ�Ϸ�
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it))//������ĸ����
			return false;
	return true;
}
//����
bool RsaSystem::decode(Rsa& rsa)
{
	string input_cipher_text;
	do
	{
		cout << ">����16��������:";
	} while (!(cin >> input_cipher_text));

	if (!isLegal(input_cipher_text))
		return false;

	LargeInt cipher_text(input_cipher_text);

	long t1 = clock();
	LargeInt plain_text = rsa.Decode(cipher_text);
	long t2 = clock();
	cout << "��ʱ:" << (t2 - t1) << "ms." << endl;

	cout << "����:" << cipher_text << endl
		<< "����:" << plain_text << endl;
	return true;
}
//����
bool RsaSystem::encrypt(Rsa& rsa)
{
	string input_plain_text;
	do
	{
		cout << ">����16��������:";
	} while (!(cin >> input_plain_text));

	if (!isLegal(input_plain_text))
		return false;

	LargeInt plain_text(input_plain_text);
	LargeInt cipher_text = rsa.Encrypt(plain_text);

	cout << "����:" << plain_text << endl
		<< "����:" << cipher_text << endl;

	return true;
}

void RsaSystem::print(Rsa& rsa)
{//���
	cout << rsa << endl;
}

//���ƺ���
void RsaSystem::Start()
{
	string input_choice;
	int modulus_bits = 0;

	cout << "����ģ����λ��:";
	cin >> modulus_bits;
	int prime_number_length = modulus_bits / 2;

	cout << "��ʼ��...." << endl;
	long t1 = clock();
	Rsa rsa(prime_number_length);
	long t2 = clock();
	cout << "��ʼ�����." << endl;
	cout << "��ʱ:" << (t2 - t1) << "ms." << endl;

	while (true)
	{
		menu();//�˵���ʾ
		cout << ">";
		if (!(cin >> input_choice))
			cerr << "δ�ܳɹ�����" << endl;

		switch (input_choice[0])
		{
		case 'e':
		case 'E':
			encrypt(rsa);//����
			break;
		case 'd':
		case 'D':
			decode(rsa);//����
			break;
		case 's':
		case 'S':
			Start();//���¿�ʼ��ʼ
			break;
		case 'p':
		case 'P':
			print(rsa);//�����˽Կ��Ϣ
			break;
		case 'q':
		case 'Q':
			return;

		default:
			cerr << "���������" << endl;
		}
	}
}