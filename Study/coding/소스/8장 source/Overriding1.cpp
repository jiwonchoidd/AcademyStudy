/*
   Overriding1.cpp
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
	void fct(){ //AAA Ŭ������ fct() �Լ��� �������̵�.
		cout<<"BBB"<<endl;
	}
};

int main(void)
{
	BBB b;
	b.fct();

	return 0;
}