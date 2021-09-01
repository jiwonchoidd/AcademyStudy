/*
   Default3.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class Point 
{
	int x, y;
public:
	Point(int _x=0, int _y=0)
	{
		x=_x, y=_y;
	}
	void ShowData()
	{
		cout<<x<<' '<<y<<endl;
	}
};

int main()
{
	Point p1(10, 20);
	p1.ShowData();

	Point p2;   // Point(0, 0)과 같다.
	p2.ShowData(); 

	Point p3(10);   // Point(10, 0)과 같다.
	p3.ShowData();

	return 0;
}
