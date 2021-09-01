/*
	ref_error.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

int& function()
{
	int val=10;
	return val;
}

int main(void)
{
	int &ref=function();
	cout<<ref<<endl;

	return 0;
}