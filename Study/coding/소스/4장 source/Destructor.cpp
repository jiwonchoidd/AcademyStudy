/*
   Destructor.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class AAA
{
public:
	AAA() // 생성자.
	{ 
		cout<<"생성자 호출"<<endl;
	}
	~AAA() // 소멸자.
	{
		cout<<"소멸자 호출"<<endl;
	}
};

int main()
{
	AAA aaa1;
	AAA aaa2;

	return 0;
}