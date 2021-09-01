/*
	FunctionOverloading.cpp
*/

#include <iostream>

void function(void){
	std::cout<<"function(void) call"<<std::endl;
}

void function(char c){
	std::cout<<"function(char c) call"<<std::endl;
}

void function(int a, int b){
	std::cout<<"function(int a, int b) call"<<std::endl;
}

int main(void)
{
	function();
	function('a');
	function(12, 13);

	return 0;
}