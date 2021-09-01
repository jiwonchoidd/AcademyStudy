/*
   mutable.cpp
*/
#include<iostream>
using std::cout;
using std::endl;
class Parent
{
public:
	//mutable int val1;
	//mutable int val2;
	int val1;
	int val2;

}:

class AAA : public Parent
{
private:
public:
	void SetData(int a, int b) const 
	{
		val1=a;  // val1이 mutable이므로 OK!
		val2=b;  // Error!
	}
};

int main(void)
{
	AAA a1;
	a1.SetData(10, 20);
	return 0;
}