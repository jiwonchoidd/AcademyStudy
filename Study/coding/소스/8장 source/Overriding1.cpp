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
	void fct(){ //AAA 클래스의 fct() 함수를 오버라이딩.
		cout<<"BBB"<<endl;
	}
};

int main(void)
{
	BBB b;
	b.fct();

	return 0;
}