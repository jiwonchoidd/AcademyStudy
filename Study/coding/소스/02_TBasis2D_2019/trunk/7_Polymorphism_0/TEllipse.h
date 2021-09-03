#pragma once
#include "TFigure.h"
class TEllipse : public TFigure
{
public:
	void Draw();		// 오버라이딩
public:
	TEllipse();
	TEllipse(int x, int y, int width, int height);
	~TEllipse();
};

