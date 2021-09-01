/*
   OpOverloading4.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

class Point {
private:
	int x, y;
public:
	Point(int _x=0, int _y=0):x(_x), y(_y){}
	void ShowPosition();
	Point& operator++();
};
void Point::ShowPosition(){ 
	cout<<x<<" "<<y<<endl; 
}
Point& Point::operator++(){
	x++;
	y++;
	return *this;
}

int main(void)
{
	Point p1(1, 2);	
	(p1++).ShowPosition();

	Point p2(1, 2);
	(++p2).ShowPosition();
	return 0;
}

