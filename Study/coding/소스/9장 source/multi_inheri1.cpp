/*
    multi_inheri1.cpp
*/

#include <iostream>
using std::cout;
using std::endl;

class AAA{
public:
	void String1(){
		cout<<"AAA::String1"<<endl;
	}
};

class BBB{
public:
	void String2(){
		cout<<"BBB::String2"<<endl;
	}
};

class CCC : public AAA, public BBB{
public:
	void ShowString(){
		String1();
		String2();
	}
};

int main(void)
{
	CCC ccc;
	ccc.ShowString();

	return 0;
}