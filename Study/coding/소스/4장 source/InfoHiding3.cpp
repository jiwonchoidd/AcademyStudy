/*
   InfoHiding3.cpp
*/
#include<iostream>
using std::cout;
using std::endl;
using std::cin;

class Point
{
	int x;   // x��ǥ�� ���� : 0 ~ 100
	int y;   // y��ǥ�� ���� : 0 ~ 100
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
		cout<<"X��ǥ �Է� ����, Ȯ�� ���"<<endl;
		return;
	}
	x=_x;
}
void Point::SetY(int _y)
{
	if(_y<0 || _y>100)
	{
		cout<<"Y��ǥ �Է� ����, Ȯ�� ���"<<endl;
		return;
	}
	y=_y;
}


int main()
{
	int x, y;
	cout<<"��ǥ�Է� : ";
	cin>>x>>y;

	Point p;
	p.SetX(x);
	p.SetY(y);

	cout<<"�Է� �� �����͸� �̿��ؼ� �׸��� �׸�"<<endl;
	return 0;
}
