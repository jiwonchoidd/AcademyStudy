#include "TEllipse.h"

void TEllipse::Draw()
{
	cout << "Draw Ellipse: ";
	cout << "(" << x << ", " << y << "), ";
	cout << width << " x " << height;
	cout << endl;
}

TEllipse::TEllipse()
{
	cout << "TEllipse 持失切" << endl;
}
TEllipse::TEllipse(int x, int y, int width, int height)
	: TFigure(x, y, width, height)
{
	cout << "TEllipse 持失切" << endl;
}

TEllipse::~TEllipse()
{
	cout << "TEllipse 背薦切" << endl;
}
