/*
	DefaultParameter1.cpp
*/

#include <iostream>

int function(int a=0){
	return a+1;
}

int main(void)
{
	std::cout<<function(11)<<std::endl;
	std::cout<<function()<<std::endl;

	return 0;
}