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
		cout<<count++<<"��° Person ��ü ����"<<endl;
	}
	void ShowData(){
		cout<<"�̸�: "<<name;
		cout<<"����: "<<age;
	}
};

int Person::count=1; // static ��� �ʱ�ȭ

int main(void)
{
	Person::count = 10;

	Person p1("Lee", 13);
	Person p2("Hong", 22);

	return 0;
}