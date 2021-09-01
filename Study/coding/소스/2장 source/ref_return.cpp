/*
	ref_return.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

int& increment(int &val)
{
	val++;
	return val;
}

int main(void)
{
	int n=10;
	int &ref=increment(n);

	cout<<"n  : "<<n<<endl;
	cout<<"ref: "<<ref<<endl;

	return 0;
}