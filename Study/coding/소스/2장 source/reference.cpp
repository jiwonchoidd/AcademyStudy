/*
	reference.cpp
*/

#include <iostream>

using std::cout;
using std::endl;

int main(void)
{
	int val=10;
	int &ref=val;

	val++;
	cout<<"ref : "<<ref<<endl;
	cout<<"val : "<<val<<endl;

	ref++;
	cout<<"ref : "<<ref<<endl;
	cout<<"val : "<<val<<endl;

	return 0;
}