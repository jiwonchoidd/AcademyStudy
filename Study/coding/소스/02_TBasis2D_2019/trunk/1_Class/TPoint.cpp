#include "TPoint.h"


void TPoint::SetPosition(int _x, int _y)
{
	x = _x;
	y = _y;
}

void TPoint::Move(int _x, int _y)
{
	x += _x;
	y += _y;
}

void TPoint::Show(void)
{
	cout << "(" << x << ", " << y << ")" << endl;
}

TPoint::TPoint()
{
	cout << "Constuctor" << endl;

}


TPoint::~TPoint()
{
	cout << "Distructor" << endl;

}
