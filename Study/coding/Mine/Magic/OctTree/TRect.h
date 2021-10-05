#pragma once
#include "TVector.h"
struct Rect
{
	float  x, y, z, w, h;
};
struct TRect
{
	TVector p0;
	TVector p1;
	TVector wh;
};