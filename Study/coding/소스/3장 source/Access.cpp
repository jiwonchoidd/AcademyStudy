/*
	Access.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

class Counter {
public:
	int val;
	void Increment(void)
	{
		val++; //���� ����.
	}
};

int main(void)
{
	Counter cnt;
	cnt.val=0; //�ܺ� ����.
	cnt.Increment(); //�ܺ� ����.
	cout<<cnt.val<<endl; //�ܺ� ����.

	return 0;
}