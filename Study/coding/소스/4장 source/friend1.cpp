/*
   friend1.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

class Counter
{
private:
	int val;

public:
	Counter() { 
		val=0;
	}
	void Print() const {
		cout<<val<<endl;
	}
		
	friend void SetX(Counter& c, int val);  //friend ����.
};

void SetX(Counter& c, int val) // �����Լ�.
{
	c.val=val;
}

int main()
{
	Counter cnt;
	cnt.Print();

	SetX(cnt, 2002);
	cnt.Print();

	return 0;
}
