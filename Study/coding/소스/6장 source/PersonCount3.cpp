/*
   PersonCount3.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class Person
{
	char name[20];
	int age;
	static int count;
public:
	Person(char* _name, int _age)
	{
		strcpy(name, _name);
		age=_age;
		cout<<count++<<"번째 Person 객체 생성"<<endl;
	}
	void ShowData(){
		cout<<"이름: "<<name;
		cout<<"나이: "<<age;
	}
};

int Person::count=1; // static 멤버 초기화

int main(void)
{
	Person::count = 10;

	Person p1("Lee", 13);
	Person p2("Hong", 22);

	return 0;
}