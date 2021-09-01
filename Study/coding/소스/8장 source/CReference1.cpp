/*
   CReference1.cpp
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
	Student& ref1=p;
	Person& ref2=p;
	
	p.Sleep();
	ref1.Sleep();
	ref2.Sleep();

	return 0;
}