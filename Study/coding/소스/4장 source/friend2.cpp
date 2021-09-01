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
	friend class BBB;  // class BBB를 friend로 선언함!
};

class BBB
{
public:
	void SetData(AAA& aaa, int val){
		aaa.data=val; //class AAA의 private 영역 접근!
	}
};

int main()
{
	AAA aaa;
	BBB bbb;

	bbb.SetData(aaa, 10);

	return 0;
}
