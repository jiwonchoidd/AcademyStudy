/*
   CPointer2.cpp
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
	Person* p3=new PartTimeStd;

	p3->Sleep();
//	p3->Study(); // Error�� ����
//	p3->Work();  // Error�� ����

	return 0;
}