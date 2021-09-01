/*
   Person3.cpp
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
	Person(char* _name, char* _phone, int _age);
	void ShowData();
};

Person::Person(char* _name, char* _phone, int _age)
{
	strcpy(name, _name);
	strcpy(phone, _phone);
	age=_age;
}
void Person::ShowData()
{
	cout<<"name: "<<name<<endl;
	cout<<"phone: "<<phone<<endl;
	cout<<"age: "<<age<<endl;
}

int main()
{
	Person p("KIM", "013-333-5555", 22);
	p.ShowData();

	return 0;
}
