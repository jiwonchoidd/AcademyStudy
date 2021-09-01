/*
   CPointer1.cpp
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
	Person* p1=new Person;
	Person* p2=new Student;
	Person* p3=new PartTimeStd;

	p1->Sleep();
	p2->Sleep();
	p3->Sleep();

	return 0;
}