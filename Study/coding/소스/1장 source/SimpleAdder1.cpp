/*
	SimpleAdder1.cpp
*/

#include <iostream.h>

int main(void)
{
	int val1, val2;

	cout<<"첫번째 숫자 입력 : ";
	cin>>val1;
	
	cout<<"두번째 숫자 입력 : ";
	cin>>val2;

	int result=val1+val2;
	cout<<"덧셈 결과 : "<<result<<endl;

	return 0;
}