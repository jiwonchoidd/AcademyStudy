/*
   Overriding4.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class AAA
{
public:
	virtual void fct(){
		cout<<"AAA"<<endl;
	}
};

class BBB : public AAA
{
public:
	void fct(){    // virtual void fct()
		cout<<"BBB"<<endl;
	}
};

class CCC : public BBB
{
public:
	void fct(){
		cout<<"CCC"<<endl;
	}
};


int main(void)
{
	BBB* b=new CCC;
	b->fct();

	AAA* a=b;
	a->fct();

	delete b;
	return 0;
}
