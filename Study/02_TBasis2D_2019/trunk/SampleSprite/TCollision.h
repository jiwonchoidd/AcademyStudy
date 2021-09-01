#pragma once
#include <windows.h>
#include <math.h>

class TCollision
{
public:
	bool		RectInPt(RECT rt, POINT pt);
	bool		RectInRect(RECT rtDesk, RECT rtSrc);
	bool		SphereInSphere(RECT rtDesk, RECT rtSrc);
public:
	TCollision();
	virtual ~TCollision();
};

