/*
   friend2.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

class AAA
{
private:
	int data;
	friend class BBB;  // class BBB�� friend�� ������!
};

class BBB
{
public:
	void SetData(AAA& aaa, int val){
		aaa.data=val; //class AAA�� private ���� ����!
	}
};

int main()
{
	AAA aaa;
	BBB bbb;

	bbb.SetData(aaa, 10);

	return 0;
}
