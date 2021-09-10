#pragma once
#include "TEmployee.h"
class TTemporary : public TEmployee
{
private:
	int time; //일한시간
	int pay; //시간당 급여.
public:
	TTemporary(char* _name, int _time, int _pay);
	int GetPay();
};

