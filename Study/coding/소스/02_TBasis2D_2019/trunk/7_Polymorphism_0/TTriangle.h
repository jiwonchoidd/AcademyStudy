#pragma once
#include "TFigure.h"
class TTriangle : public TFigure
{
public:
	void Draw();		// 오버라이딩
public:
	TTriangle();
	TTriangle(int x, int y, int width, int height);
	~TTriangle();
};

