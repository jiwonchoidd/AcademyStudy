/*
   OverInternal.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

class A
{
	int a;
	int b;
public:
	virtual void fct1(){
		cout<<"fct1(...)"<<endl;
	}
	virtual void fct2(){
		cout<<"fct2(...)"<<endl;
	}	
};

class B : public A
{
	int c;
	int d;
public:
	virtual void fct1(){
		cout<<"overriding fct1(...)"<<endl;
	}
	void fct3(){
		cout<<"fct3(...)"<<endl;
	}
};

int main(void)
{
	A* aaa=new A();
	aaa->fct1();

	B* bbb=new B();
	bbb->fct1();
	
	return 0;
}