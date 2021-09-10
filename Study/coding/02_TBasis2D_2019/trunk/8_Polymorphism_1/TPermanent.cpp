#include "TPermanent.h"

TPermanent::TPermanent(char* _name, int sal): TEmployee(_name)
{
	salary = sal;
}
int TPermanent::GetPay()
{
	return salary;
}
TPermanent::~TPermanent()
{
}
