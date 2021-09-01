/*
   OpIntro.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

// class iValue{ int k ;);// 3
// class jValue int k ;); // 5
//int Add = iValue + jValue

//int  iValue  =3;
//int  jValue  = 5;
//int  Hab = iValue + jValue;

class Point {
private:
	int x, y;
public:
	Point(int _x=0, int _y=0):x(_x), y(_y){}
	void ShowPosition();
	void operator+(int val); //operator+라는 이름의 함수
};
void Point::ShowPosition() { 
	cout<<x<<" "<<y<<endl; 
}
void Point::operator+(int val) {
	x+=val;
	y+=val;
}

int main(void)
{
	Point p(3, 4);
	p.ShowPosition();

	p.operator+(10);
	p.ShowPosition();
	return 0;
}
