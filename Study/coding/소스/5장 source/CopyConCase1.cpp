/*
   CopyConCase1.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class AAA
{
public:
	AAA(){
		cout<<"AAA() 호출"<<endl;
	}
	AAA(const AAA& a){
		cout<<"AAA(const A& a) 호출"<<endl;
	}
};

int main()
{
	AAA obj1;
	AAA obj2=obj1;

	return 0;
}
