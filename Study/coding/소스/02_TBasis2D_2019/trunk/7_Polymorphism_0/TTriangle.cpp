#include "TTriangle.h"
void TTriangle::Draw()
{
	cout << "Draw TTriangle: ";
	cout << "(" << x << ", " << y << "), ";
	cout << width << " x " << height;
	cout << endl;
}

TTriangle::TTriangle()
{
	cout << "TTriangle 持失切" << endl;
}
TTriangle::TTriangle(int x, int y, int width, int height)
	: TFigure(x, y, width, height)
{
	cout << "TTriangle 持失切" << endl;
}

TTriangle::~TTriangle()
{
	cout << "TTriangle 背薦切" << endl;
}

