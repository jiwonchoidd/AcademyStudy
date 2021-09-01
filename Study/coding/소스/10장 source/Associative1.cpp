/*
   Associative1.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

class Point {
private:
	int x, y;
public:
	Point(int _x=0, int _y=0):x(_x), y(_y){}
	void ShowPosition();
	Point operator+(int val); //operator+라는 이름의 함수
};
void Point::ShowPosition() { 
	cout<<x<<" "<<y<<endl; 
}
Point Point::operator+(int val) {
	Point temp(x+val, y+val);
	return temp;
}

int main(void)
{
	Point p1(1, 2);
	Point p2=p1+3;
	p2.ShowPosition();

	return 0;
}
