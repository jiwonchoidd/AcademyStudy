/*
   handling.cpp   
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
	if(b==0){
		cout<<"입력오류! 다시 실행 하세요."<<endl;
	}
	else {
		cout<<"a/b의 몫 : "<<a/b<<endl;
		cout<<"a/b의 나머지 : "<<a%b<<endl;
	}
	
	return 0;
}