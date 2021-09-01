/*
   IntroTemplate1.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

template <typename T>
T Add(T a, T b) 
{
	return a+b;
}

int main(void)
{
	cout<<Add(10, 20)<<endl;
	cout<<Add(1.1, 2.2)<<endl;

	return 0;
}
