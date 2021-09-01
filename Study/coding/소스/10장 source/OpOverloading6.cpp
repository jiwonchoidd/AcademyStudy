/*
   OpOverloading6.cpp   
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
	os<<"["<<p.x<<", "<<p.y<<"]"<<endl;	
	return os;
}

int main(void)
{
	Point p(1, 3);
	cout<<p;

	return 0;
}
