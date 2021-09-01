/*
   BasicInheri2.cpp
*/

#include <iostream>
using std::endl;
using std::cout;

class Person
{
	int age;
	char name[20];
public:

	int GetAge() const {
		return age;
	}
	const char* GetName() const {
		return name;
	}

	Person(int _age=1, char* _name="noname"){
		age=_age;
		strcpy(name, _name);
	}
};

class Student: public Person
{
	char major[20]; //전공
public:
	Student(int _age, char* _name, char* _major){
		age=_age;
		strcpy(name, _name);
		strcpy(major, _major);
	}
	const char* GetMajor() const {
		return major;
	}
	void ShowData() const {
		cout<<"이름: "<<GetName()<<endl;
		cout<<"나이: "<<GetAge()<<endl;
		cout<<"전공: "<<GetMajor()<<endl;
	}
};

int main(void)
{
	Student Kim(20, "Hong Gil Dong", "computer");
	Kim.ShowData();

	return 0;
};