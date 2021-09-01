/*
   CopyCon4.cpp
   Person6.cpp에서 정의한 Person 클래스
*/
#include<iostream>
using std::cout;
using std::endl;

class Person
{
	char *name;
	char *phone;
	int age;
public:
	Person(char* _name, char* _phone, int _age);
	~Person();
	void ShowData();
};

Person::Person(char* _name, char* _phone, int _age)
{
	name=new char[strlen(_name)+1];
	strcpy(name, _name);

	phone=new char[strlen(_phone)+1];
	strcpy(phone, _phone);

	age=_age;
}
Person::~Person()
{
	delete []name;
	delete []phone;
}
void Person::ShowData()
{
	cout<<"name: "<<name<<endl;
	cout<<"phone: "<<phone<<endl;
	cout<<"age: "<<age<<endl;
}

int main()
{
	Person p1("KIM", "013-333-5555", 22);
	Person p2=p1;
	
	return 0;
}
