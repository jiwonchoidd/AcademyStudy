/*
   Overriding5.cpp
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
	void fct(){
		AAA::fct();  // 방법 1. 
		cout<<"BBB"<<endl;
	}
};


int main(void)
{
	AAA* a=new BBB;
	cout<<"첫 번째 시도"<<endl;
	a->fct();  

	cout<<"두 번째 시도"<<endl;
	a->AAA::fct(); // 방법 2.

	return 0;
}
