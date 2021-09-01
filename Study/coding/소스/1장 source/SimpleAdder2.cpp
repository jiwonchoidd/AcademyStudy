/*
	SimpleAdder2.cpp
*/

#include <iostream>

int main(void)
{
	int val1, val2;

	std::cout<<"첫번째 숫자 입력 : ";
	std::cin>>val1;
	
	std::cout<<"두번째 숫자 입력 : ";
	std::cin>>val2;

	int result=val1+val2;
	std::cout<<"덧셈 결과 : "<<result<<std::endl;

	return 0;
}