/*
   CopyConCase3.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class AAA
{
	int val;
public:
	AAA(int i){
		val=i;
	}
	AAA(const AAA& a){
		cout<<"AAA(const A& a) È£Ãâ"<<endl;
		val=a.val;
	}
	void ShowData(){
		cout<<"val: "<<val<<endl;
	}
};

AAA* function(void)
{
	AAA a(10);
	return &a;
}

int main()
{
	AAA* pTemp =function(); //function().ShowData();

	return 0;
}
