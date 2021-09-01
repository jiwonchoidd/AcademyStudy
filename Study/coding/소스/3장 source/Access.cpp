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
		val++; //郴何 立辟.
	}
};

int main(void)
{
	Counter cnt;
	cnt.val=0; //寇何 立辟.
	cnt.Increment(); //寇何 立辟.
	cout<<cnt.val<<endl; //寇何 立辟.

	return 0;
}