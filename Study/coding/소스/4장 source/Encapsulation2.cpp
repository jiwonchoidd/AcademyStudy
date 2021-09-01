/*
   Encapsulation2.cpp
*/
#include<iostream>
using std::cout;
using std::endl;
using std::cin;

class Point
{
	int x;   // xÁÂÇ¥ÀÇ ¹üÀ§ : 0~100
	int y;   // yÁÂÇ¥ÀÇ ¹üÀ§ : 0~100
public:
	int GetX(){	return x; }
	int GetY(){	return y; }

	void SetX(int _x);
	void SetY(int _y);

	void ShowData();  //Ä¸½¶È­¸¦ À§ÇØ Ãß°¡µÈ ÇÔ¼ö.
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

void Point::ShowData()
{
	cout<<"xÁÂÇ¥: "<<x<<endl;
	cout<<"yÁÂÇ¥: "<<y<<endl;
}

int main()
{
	int x, y;
	cout<<"ÁÂÇ¥ÀÔ·Â : ";
	cin>>x>>y;

	Point p;
	p.SetX(x);
	p.SetY(y);
	p.ShowData();

	return 0;
}
