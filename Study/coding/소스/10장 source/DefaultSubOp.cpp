/*
   DefaultSubOp.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;
using std::ostream;

class Point {
private:
	int x, y;
public:
	Point(int _x=0, int _y=0):x(_x), y(_y){}
	friend ostream& operator<<(ostream& os, const Point& p);
};

ostream& operator<<(ostream& os, const Point& p)
{
	os<<"["<<p.x<<", "<<p.y<<"]";	
	return os;
}


int main(void)
{
	Point p1(1, 3);
	Point p2(10, 30);
	cout<<p1<<endl;
	cout<<p2<<endl;

	p1=p2;  // p1.operator=(p2)
	cout<<p1<<endl;

	return 0;
}