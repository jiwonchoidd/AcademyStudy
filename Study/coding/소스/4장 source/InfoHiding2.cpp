/*
   InfoHiding2.cpp
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

	void SetX(int _x){ x=_x; }
	void SetY(int _y){ y=_y; }
};

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
