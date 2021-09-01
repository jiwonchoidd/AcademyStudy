/*
   OpOverloading3.cpp   
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
	friend Point& operator--(Point& p);
};
void Point::ShowPosition(){ 
	cout<<x<<" "<<y<<endl; 
}
Point& Point::operator++(){
	x++;
	y++;
	return *this;
}

Point& operator--(Point& p){
	p.x--;
	p.y--;
	return p;
}

int main(void)
{
	Point p(1, 2);
	++p; //p의 x, y 값을 1씩 증가.
	p.ShowPosition();  //2, 3

	--p; //p의 x, y 값을 2씩 증가.
	p.ShowPosition();  //1, 2

	++(++p);
	p.ShowPosition();  //3, 4

	--(--p);
	p.ShowPosition();  //1, 2

	return 0;
}

