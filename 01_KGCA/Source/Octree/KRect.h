#pragma once
#include "KVector.h"
struct Rect
{
	float  x, y, z, w, h;
};
struct KRect
{
	KVector p0;
	KVector p1;
	KVector wh;
};