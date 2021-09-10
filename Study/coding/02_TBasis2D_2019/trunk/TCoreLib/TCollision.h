#pragma once
#include "TStd.h"
class TCollision
{
public:
	static bool RectInPt(RECT rt, POINT pt);
	static bool RectInRect(RECT rt1, RECT rt2);
	static bool SphereInSphere(RECT rt1, RECT rt2);
	static bool SphereInPoint(TSphere rt, TPoint pos);
	static bool SphereInPoint(TSphere rt, POINT pos);
public:
	TCollision();
	~TCollision();
};

