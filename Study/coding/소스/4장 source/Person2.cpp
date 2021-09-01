/*
   Person2.cpp
*/
#include<iostream>
using std::cout;
using std::endl;
using std::cin;

const int SIZE=20;

class Person
{
	char name[SIZE];
	char phone[SIZE];
	int age;
public:
	void ShowData();
	void SetData(char* _name, char* _phone, int _age);
};

void Person::ShowData()
{
	cout<<"name: "<<name<<endl;
	cout<<"phone: "<<phone<<endl;
	cout<<"age: "<<age<<endl;
}

void Person::SetData(char* _name, char* _phone, int _age)
{
	strcpy(name, _name);
	strcpy(phone, _phone);
	age=_age;
}

int main()
{
	Person p;
	p.SetData("KIM", "013-333-5555", 22);
	p.ShowData();

	return 0;
}
