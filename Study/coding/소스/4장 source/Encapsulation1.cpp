/*
   Encapsulation1.cpp
*/
#include<iostream>
using std::cout;
using std::endl;
using std::cin;

class Point
{
	int x;   // xÁÂÇ¥ÀÇ ¹üÀ§ : 0 ~ 100
	int y;   // yÁÂÇ¥ÀÇ ¹üÀ§ : 0 ~ 100
public:
	int GetX(){	return x; }
	int GetY(){	return y; }

	void SetX(int _x);
	void SetY(int _y);
};

void Point::SetX(int _x)
{
	if(_x<0 || _x>100) {
		cout<<"XÁÂÇ¥ ÀÔ·Â ¿À·ù, È®ÀÎ ¿ä¸Á"<<endl;
		return;
	}
	x=_x;
}
void Point::SetY(int _y)
{
	if(_y<0 || _y>100)
	{
		cout<<"YÁÂÇ¥ ÀÔ·Â ¿À·ù, È®ÀÎ ¿ä¸Á"<<endl;
		return;
	}
	y=_y;
}

class PointShow
{
public:
	void ShowData(Point p)
	{
		cout<<"xÁÂÇ¥: "<<p.GetX()<<endl;
		cout<<"yÁÂÇ¥: "<<p.GetY()<<endl;
	}

};


int main()
{
	int x, y;
	cout<<"ÁÂÇ¥ÀÔ·Â : ";
	cin>>x>>y;

	Point p;
	p.SetX(x);
	p.SetY(y);

	PointShow show;
	show.ShowData(p);

	return 0;
}
