/*
   CReference2.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class Person
{
public:
	void Sleep(){ 
		cout<<"Sleep"<<endl;
	}
};

class Student : public Person
{
public:
	void Study(){
		cout<<"Study"<<endl;
	}
};

class PartTimeStd : public Student
{
public:
	void Work(){
		cout<<"Work"<<endl;
	}
};

int main(void)
{
	PartTimeStd p;
	p.Sleep();
	p.Study();
	p.Work();

	Person& ref=p;
	ref.Sleep();
//	ref.Study(); // Error�� ����
//	ref.Work();  // Error�� ����

	return 0;
}