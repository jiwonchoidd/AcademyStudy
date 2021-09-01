/*
   CopyCon3.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class Point
{
	int x, y;
public:
	Point(int _x, int _y){
		x=_x;
		y=_y;
	}
	Point(const Point& p){
		x=p.x;
		y=p.y;
	}
	void ShowData(){
		cout<<x<<' '<<y<<endl;
	}
};

int main(void)
{
	Point p1(10, 20);
	Point p2(p1);

	p1.ShowData();
	p2.ShowData();

	return 0;
}