/*
	BetweenAdder.cpp
*/

#include <iostream.h>

int main(void)
{
	int val1, val2;
	int result=0;

	cout<<"두개의 숫자 입력 : ";
	cin>>val1>>val2;

	if(val1<val2)	//val2가 크다면.
	{
		for(int i=val1+1; i<val2; i++)
			result+=i;
	}
	else			//val1이 크다면.
	{
		for(int i=val2+1; i<val1; i++)
			result+=i;
	}

	cout<<"두 수 사이에 존재하는 정수의 합 : "<<result<<endl;

	return 0;
}