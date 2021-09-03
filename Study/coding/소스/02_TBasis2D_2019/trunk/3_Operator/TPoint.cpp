#include "TPoint.h"

TPoint TPoint::operator++(void)	// 전위형
{
	++x, ++y;
	return *this;
}
TPoint TPoint::operator++(int)		// 후위형
{
	TPoint temp = *this;
	++x, ++y;
	return temp;
}
TPoint TPoint::operator+(const TPoint &pt) const {
	return TPoint(x + pt.x, y + pt.y);
}
TPoint TPoint::operator-(const TPoint &pt) const {
	return TPoint(x - pt.x, y - pt.y);
}
TPoint TPoint::operator*(int mag) const {
	return TPoint(x * mag, y * mag);
}
TPoint TPoint::operator/(int div) const {
	return TPoint(x / div, y / div);
}
TPoint operator*(int mag, const TPoint &pt) {
	return TPoint(pt.x * mag, pt.y * mag);
}
TPoint operator/(int div, const TPoint &pt) {
	return TPoint(pt.x / div, pt.y / div);
}
TPoint &TPoint::operator=(const TPoint &pt) {
	x = pt.x; y = pt.y;
	return *this;
}
TPoint &TPoint::operator+=(const TPoint &pt) {
	x += pt.x; y += pt.y;
	return *this;
}
TPoint &TPoint::operator-=(const TPoint &pt) {
	x -= pt.x; y -= pt.y;
	return *this;
}
TPoint &TPoint::operator*=(const TPoint &pt) {
	x *= pt.x; y *= pt.y;
	return *this;
}
TPoint &TPoint::operator/=(const TPoint &pt) {
	x /= pt.x; y /= pt.y;
	return *this;
}
TPoint &TPoint::operator*=(int mag) {
	x *= mag; y *= mag;
	return *this;
}
TPoint &TPoint::operator/=(int div) {
	x /= div; y /= div;
	return *this;
}

bool TPoint::operator==(const TPoint &pt) const
{
	return (x == pt.x && y == pt.y);	// x, y가 모두 같으면 true
}
bool TPoint::operator!=(const TPoint &pt) const
{
	return (x != pt.x || y != pt.y);	// x, y 중 하나라도 다르면 true
}
int &TPoint::operator[](int index)
{
	if (index == 0)
		return x;
	return y;
}
int &TPoint::operator()(int index)
{
	if (index == 0)
		return x;
	return y;
}

void *TPoint::operator new(size_t size)
{
	return malloc(size);
}
void TPoint::operator delete(void *p)
{
	free(p);
}
ostream &operator<<(ostream &os, const TPoint &pt) {
	os << "(" << pt.x << ", " << pt.y << ")";
	return os;
}
istream &operator>>(istream &is, TPoint &pt) {
	is >> pt.x;
	is >> pt.y;
	return is;
}
TPoint TPoint::operator*(const TPoint &c)
{
	TPoint temp;
	temp.x = x*c.x;
	temp.y = y*c.y;
	return temp;
}

TPoint TPoint::operator/(const TPoint &c)
{
	TPoint temp;
	temp.x = x/c.y;
	temp.y = y/c.y;
	return temp;
}

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
TPoint::TPoint(int x, int y)
{
	this->x = x;
	this->y = y;
	cout << "Constuctor" << endl;
}


TPoint::~TPoint()
{
	cout << "Distructor" << endl;

}
