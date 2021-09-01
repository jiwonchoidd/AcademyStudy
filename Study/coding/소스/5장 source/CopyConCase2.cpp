/*
   CopyConCase2.cpp
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

void function(AAA a=obj)
{
	a.ShowData();
}

int main()
{
	AAA obj(30);
	function(obj);

	return 0;
}
