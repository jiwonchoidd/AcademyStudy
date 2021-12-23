#pragma once
#include "KVector3.h"
struct Rect
{
	float  x, y, z, w, h;
};
struct KRect
{
	KVector3 p0;
	KVector3 p1;
	KVector3 wh;
};