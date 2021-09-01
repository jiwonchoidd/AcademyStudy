/*
   ConstMember1.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class Student
{
	const int id;
	int age;
	char name[20];
	char major[30];
public:
	Student(int _id, int _age, char* _name, char* _major)
	{
		id=_id;
		age=_age;
		strcpy(name, _name);
		strcpy(major, _major);
	}

	void ShowData()
	{
		cout<<"�̸�: "<<name<<endl;
		cout<<"����: "<<age<<endl;
		cout<<"�й�: "<<id<<endl;
		cout<<"�а�: "<<major<<endl;
	}
};

int main()
{
	Student Kim(200577065, 20, "Kim Gil Dong", "Computer Eng.");
	Student Hong(200512065, 19, "Hong Gil Dong", "Electronics Eng.");

	Kim.ShowData();
	cout<<endl;
	Hong.ShowData();

	return 0;
}
