/*
   explicit.cpp
*/

#include<iostream>
using std::cout;
using std::endl;

class AAA
{
public:
    explicit AAA(int n){
		cout<<"explicit AAA(int n)"<<endl;
	}
};

int main(void)
{
	AAA a1=10;

	return 0;
}