/*
   Constructor1.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

const int SIZE=20;

class AAA
{
	int i, j;
public:
	AAA() //������.
	{ 
		cout<<"������ ȣ��"<<endl;
		i=10, j=20;
	}
	void ShowData()
	{
		cout<<i<<' '<<j<<endl;
	}
};

int main()
{
	AAA aaa;
	aaa.ShowData();

	return 0;
}
