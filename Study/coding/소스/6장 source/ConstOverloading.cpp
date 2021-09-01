/*
   ConstOverloading.cpp
*/
#include <windows.h>
#include <iostream>
using std::cout;
using std::endl;

class AAA
{
	int num;
public :
	AAA(int _num) : num(_num) {}

	void ShowData(int iValue){
		cout<<"void ShowData() 호출"<<endl;
		cout<<num<<endl;
	}
	
	int ShowData( INT iValue0 )  {
		cout<<"void ShowData() const 호출"<<endl;
		cout<<num<<endl;		
	}
};

int main()
{
	const AAA aaa1(20);
	AAA aaa2(70);

    aaa1.ShowData();
	aaa2.ShowData();
	return 0;
}
