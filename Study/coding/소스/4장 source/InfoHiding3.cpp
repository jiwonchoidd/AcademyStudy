/*
   InfoHiding3.cpp
*/
#include<iostream>
using std::cout;
using std::endl;
using std::cin;

class Point
{
	int x;   // x좌표의 범위 : 0 ~ 100
	int y;   // y좌표의 범위 : 0 ~ 100
public:
	int GetX(){	return x; }
	int GetY(){	return y; }

	void SetX(int _x);
	void SetY(int _y);
};

void Point::SetX(int _x)
{
	if(_x<0 || _x>100)	
	{
		cout<<"X좌표 입력 오류, 확인 요망"<<endl;
		return;
	}
	x=_x;
}
void Point::SetY(int _y)
{
	if(_y<0 || _y>100)
	{
		cout<<"Y좌표 입력 오류, 확인 요망"<<endl;
		return;
	}
	y=_y;
}


int main()
{
	int x, y;
	cout<<"좌표입력 : ";
	cin>>x>>y;

	Point p;
	p.SetX(x);
	p.SetY(y);

	cout<<"입력 된 데이터를 이용해서 그림을 그림"<<endl;
	return 0;
}
