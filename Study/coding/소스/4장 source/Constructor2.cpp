/*
   Constructor2.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

const int SIZE=20;

class AAA
{
	int i, j;
public:
	AAA(int _i, int _j) //»ý¼ºÀÚ.
	{ 
		i=_i, j=_j;
	}
	void ShowData()
	{
		cout<<i<<' '<<j<<endl;
	}
};

int main()
{
	AAA aaa(111, 222);
	aaa.ShowData();

	return 0;
}