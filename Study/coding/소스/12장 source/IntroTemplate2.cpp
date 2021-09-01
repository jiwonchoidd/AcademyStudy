/*
   IntroTemplate2.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

template <typename T> // 함수 템플릿 정의
void ShowData(T a, T b) 
{
	cout<<a<<endl;
	cout<<b<<endl;	
}

int main(void)
{
	ShowData(1, 2);
	ShowData(3, 2.5);

	return 0;
}
