/*
	reffunc.cpp
*/
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

struct _Person {
	int age;		// 나이 
	char name[20];	// 이름 
	char personalID[20]; // 주민등록 번호.
};

typedef struct _Person Person;

void ShowData(const Person &p)
{
	cout<<"******* 개인 정보 출력 *******"<<endl;
	cout<<"이    름 : "<<p.name<<endl;
	cout<<"주민번호 : "<<p.personalID<<endl;
	cout<<"나    이 : "<<p.age<<endl;
}

int main(void)
{
	Person man;

	cout<<"이    름 : ";
	cin>>man.name;

	cout<<"나    이 : ";
	cin>>man.age;

	cout<<"주민번호 : ";
	cin>>man.personalID;

	ShowData(man);
	return 0;
}