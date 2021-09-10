#pragma once
#include "TEmployee.h"
class TPermanent : public TEmployee
{
private:
	int salary;  // 기본급여
public:	
	int GetPay();
public:
	TPermanent(char* _name, int sal);
	~TPermanent();
};

