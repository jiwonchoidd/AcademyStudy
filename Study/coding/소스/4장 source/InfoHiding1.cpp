/*
   InfoHiding1.cpp
*/
#include<iostream>
using std::cout;
using std::endl;
using std::cin;

class Point
{
public:
	int x;   // x��ǥ�� ���� : 0 ~ 100
	int y;   // y��ǥ�� ���� : 0 ~ 100
};

int main()
{
	int x, y;
	cout<<"��ǥ�Է� : ";
	cin>>x>>y;

	Point p;
	p.x=x;
	p.y=y;

	cout<<"�Է� �� �����͸� �̿��ؼ� �׸��� �׸�"<<endl;
	return 0;
}
