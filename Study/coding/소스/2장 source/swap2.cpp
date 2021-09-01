/*
	swap2.cpp
*/

#include <iostream>

using std::cout;
using std::endl;

void swap(int &a, int &b)
{
	int temp=a;
	a=b;
	b=temp;
}

int main(void)
{
	int val1=10;
	int val2=20;

	cout<<"val1:"<<val1<<' ';
	cout<<"val2:"<<val2<<endl;

	swap(val1, val2);
	cout<<"val1:"<<val1<<' ';
	cout<<"val2:"<<val2<<endl;

	return 0;
}