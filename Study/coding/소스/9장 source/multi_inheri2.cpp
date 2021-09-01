/*
    multi_inheri2.cpp
*/

#include <iostream>
using std::cout;
using std::endl;

class AAA{
public:
	void String(){
		cout<<"AAA::String"<<endl;
	}
};

class BBB{
public:
	void String(){
		cout<<"BBB::String"<<endl;
	}
};

class CCC : public AAA, public BBB{
public:
	void ShowString(){
		String();  // AAA::String();
		String();  // BBB::String();
	}
};

int main(void)
{
	CCC ccc;
	ccc.ShowString();

	return 0;
}