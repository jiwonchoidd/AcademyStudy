/*
   Default2.cpp
*/
#include<iostream>
using std::cout;
using std::endl;

class Point 
{
	int x, y;
public:
	Point()
	{
		x=y=0;
	}
	Point(int _x, int _y)
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
	Point p1(10, 20);   //10과 20을 인자로 받는 생성자 호출
	p1.ShowData();

	Point p2;   //void 생성자 호출.
	p2.ShowData(); 
	return 0;
}
