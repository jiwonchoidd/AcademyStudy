#include "CallFuction.h"
void Func(int a, float b, bool c)
{
	int temp_a = a;
	int temp_b = b;
	int temp_c = c;
	std::cout << a << b << c<<std::endl;
}
int main()
{
	auto FuncA = std::bind(Func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_1);
	FuncA(1.0f,2);
	return 0;
}