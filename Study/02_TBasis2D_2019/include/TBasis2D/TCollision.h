#pragma once
#include "TStd.h"
class TCollision
{
public:
	static bool RectInPt(RECT rt, POINT pt);
	static bool Rect2Rect(RECT rt1, RECT rt2);
};

