/*
   StringClass1.cpp   
*/
#include <iostream>
#include <string>
using std::endl;
using std::cout;
using std::cin;

using std::string;

int main()
{
	string str1="Good ";
	string str2="morning";
	string str3=str1+str2;

	cout<<str1<<endl;
	cout<<str2<<endl;
	cout<<str3<<endl;

	str1+=str2;
	if(str1==str3) {   //str1과 str3의 내용 비교.	
		cout<<"equal!"<<endl;
	}

	string str4;
	cout<<"문자열 입력: ";
	cin>>str4;
	cout<<"입력한 문자열: "<<str4<<endl;

	return 0;
}