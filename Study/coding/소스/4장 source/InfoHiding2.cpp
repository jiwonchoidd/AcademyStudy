/*
   InfoHiding2.cpp
*/
#include<iostream>
using std::cout;
using std::endl;
using std::cin;

class Point
{
	int x;   // x좌표의 범위 : 0 ~ 100
	int y;   // y좌표의 점위 : 0 ~ 100
public:
	int GetX(){	return x; }
	int GetY(){	return y; }

	void SetX(int _x){ x=_x; }
	void SetY(int _y){ y=_y; }
};

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
