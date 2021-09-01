/*
   InitStyle2.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class AAA
{
	int val;
public:
	AAA(int n){
		val=n;
	}
	void ShowData(){
		cout<<val<<endl;
	}
};

int main(void)
{
	AAA a1(10);
	a1.ShowData();

	AAA a2=20;
	a2.ShowData();
	
	return 0;
}