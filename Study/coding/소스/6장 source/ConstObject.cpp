/*
   ConstObject.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

class AAA
{
	int num;
public :
	AAA(int _num) : num(_num) {}
	void Add(int n){
		num+=n;
	}
	void ShowData(){
		cout<<num<<endl;
	}
};

int main()
{
	const AAA aaa(10);
	aaa.Add(10);  // Compile Error
	aaa.ShowData();  // Compile Error

	return 0;
}
