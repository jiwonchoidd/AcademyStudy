#pragma once
#include <iostream>
using std::endl;
using std::cout;

class TEmployee
{
protected:
	char name[20];
public:
	const char* GetName();
	virtual int GetPay() = 0;
public:
	TEmployee(char* _name);
	~TEmployee();	
};

