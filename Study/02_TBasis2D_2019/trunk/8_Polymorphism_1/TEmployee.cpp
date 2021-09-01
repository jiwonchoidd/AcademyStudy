#include "TEmployee.h"


TEmployee::TEmployee(char* _name)
{
	strcpy_s(name, _name);
}
const char* TEmployee::GetName()
{
	return name;
}
TEmployee::~TEmployee()
{
}
