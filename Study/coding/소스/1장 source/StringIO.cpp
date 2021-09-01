/*
	StringIO.cpp
*/

#include <iostream.h>

int main(void)
{
	char name[100];
	char language[200];

	cout<<"이름은 무엇입니까? ";
	cin>>name;

	cout<<"좋아하는 프로그래밍 언어는 어떻게 되나요? ";
	cin>>language;

	cout<<"이름 : "<<name<<endl;
	cout<<"언어 : "<<language<<endl;

	return 0;
}