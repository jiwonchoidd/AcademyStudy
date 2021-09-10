#include "TTemporary.h"
TTemporary::TTemporary(char* _name, int _time, int _pay): TEmployee(_name)
{
	time = _time;
	pay = _pay;
}
int TTemporary::GetPay()
{
	return time*pay;
}
