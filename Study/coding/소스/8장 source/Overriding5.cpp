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
		AAA::fct();  // ��� 1. 
		cout<<"BBB"<<endl;
	}
};


int main(void)
{
	AAA* a=new BBB;
	cout<<"ù ��° �õ�"<<endl;
	a->fct();  

	cout<<"�� ��° �õ�"<<endl;
	a->AAA::fct(); // ��� 2.

	return 0;
}
