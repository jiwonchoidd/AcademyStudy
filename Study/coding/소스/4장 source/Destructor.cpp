/*
   Destructor.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class AAA
{
public:
	AAA() // ������.
	{ 
		cout<<"������ ȣ��"<<endl;
	}
	~AAA() // �Ҹ���.
	{
		cout<<"�Ҹ��� ȣ��"<<endl;
	}
};

int main()
{
	AAA aaa1;
	AAA aaa2;

	return 0;
}