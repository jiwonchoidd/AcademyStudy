#include <iostream>
#include <stdexcept>

double divideNumbers(double a, double b)
{
	if (b == 0) throw std::exception();
	return a / b;
}

void main()
{
	try {
		std::cout << divideNumbers(2.5, 0.5) << std::endl;
		std::cout << divideNumbers(2.5, 0) << std::endl;
	}catch(const std::exception& ex)
	{
		std::cout << "0으로 나누는 오류!";
	}
}