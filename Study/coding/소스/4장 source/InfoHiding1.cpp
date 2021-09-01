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
	int x;   // x좌표의 범위 : 0 ~ 100
	int y;   // y좌표의 범위 : 0 ~ 100
};

int main()
{
	int x, y;
	cout<<"좌표입력 : ";
	cin>>x>>y;

	Point p;
	p.x=x;
	p.y=y;

	cout<<"입력 된 데이터를 이용해서 그림을 그림"<<endl;
	return 0;
}
