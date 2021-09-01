/*
   Overriding2.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class AAA
{
public:
	void fct(){
		cout<<"AAA"<<endl;
	}
};

class BBB : public AAA
{
public:
	void fct(){
		cout<<"BBB"<<endl;
	}
};

int main(void)
{
	BBB* b=new BBB;
	b->fct();

	AAA* a=b;
	a->fct();

	delete b;
	return 0;
}
