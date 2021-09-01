/*
    multi_inheri3.cpp
*/

#include <iostream>
using std::cout;
using std::endl;

class AAA{
public:
	void String1(){
		cout<<"AAA::String"<<endl;
	}
};

class BBB : public AAA{   // class BBB : virtual public AAA
public:
	void String2(){
		cout<<"BBB::String"<<endl;
	}
};

class CCC : public AAA{  // class CCC : virtual public AAA
public:
	void String3(){
		cout<<"CCC::String"<<endl;
	}
};

class DDD : public BBB, public CCC {
public:
	void ShowString(){
		String1();
		String2();
		String3();
	}
};


int main(void)
{
	DDD ddd;
	ddd.ShowString();

	return 0;
}