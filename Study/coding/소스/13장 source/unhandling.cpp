/*
   unhandling.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;
using std::cin;

int main(void)
{
	int a, b;
	cout<<"두개의 숫자 입력 : ";
	cin>>a>>b;

	cout<<"a/b의 몫 : "<<a/b<<endl;
	cout<<"a/b의 나머지 : "<<a%b<<endl;
	
	return 0;
}