/*
   OpOverloading5.cpp   
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
	Point& operator++();
	Point operator++(int);
};
void Point::ShowPosition(){ 
	cout<<x<<" "<<y<<endl; 
}

Point& Point::operator++(){
	x++;
	y++;
	return *this;
}
Point Point::operator++(int){
	Point temp(x, y);  // Point temp(*this);
	x++;
	y++;
	return temp;
}


int main(void)
{
	Point p1(1, 2);	
	(p1++).ShowPosition(); // 1, 2 출력, 후 증가 이므로
	p1.ShowPosition();  // 2, 3 출력

	Point p2(1, 2);
	(++p2).ShowPosition(); // 2, 3 출력, 선 증가 이므로.
	return 0;
}

