#pragma once
#include "stdafx.h"



#include <iostream>
#include <ctime>
#include "rsa.h"
using std::cout;
using std::endl;
using std::cin;
using std::cerr;





class RsaSystem
{
public:
	void Start();
private:
	void menu();

	bool isLegal(const string & str);

	bool decode(Rsa & rsa);

	bool encrypt(Rsa & rsa);

	void print(Rsa & rsa);
};