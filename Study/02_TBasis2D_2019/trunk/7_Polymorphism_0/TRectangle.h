#pragma once
#include "TFigure.h"
class TRectangle : public TFigure
{
public:
	void Draw();		// 오버라이딩
public:
	TRectangle();
	TRectangle(int x, int y, int width, int height);
	~TRectangle();
};

