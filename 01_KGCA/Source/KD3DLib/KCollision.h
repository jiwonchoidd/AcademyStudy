#pragma once
#include "KStd.h"
enum KCollisionResult
{
	RECT_OUT = 0,
	RECT_IN,
	RECT_OVERLAP,
};
struct KRect
{
	KVector2 min;
	KVector2 max;
	KVector2 middle;
	KVector2 size;
	bool operator == (const KRect& v)
	{
		if (fabs((min - v.min).Length()) < 0.0001f)
		{
			if (fabs((max - v.max).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	KRect() {};
	KRect(KVector2 min, KVector2 max)
	{
		this->min = min;
		this->max = max;
		middle = (max + min);
		middle /= 2.0f;
		size.x = max.x - min.x;
		size.y = max.y - min.y;
	}
	KRect(KVector2 v, float w, float h)
	{
		this->min = v;
		this->max = min + KVector2(w, h);
		middle = (max + min);
		middle /= 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
struct KBox
{
	KVector3 min;
	KVector3 max;
	KVector3 middle;
	KVector3 size;
	bool operator == (const KBox& v)
	{
		if (fabs((min - v.min).Length()) < 0.0001f)
		{
			if (fabs((max - v.max).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	KBox() {};
	KBox(KVector3 min, KVector3 max)
	{
		this->min = min;
		this->max = max;
		middle = (max + min);
		middle /= 2.0f;
		size.x = max.x - min.x;
		size.y = max.y - min.y;
	}
	KBox(KVector3 v, float w, float h, float q)
	{
		this->min = v;
		this->max = min + KVector3(w, h, q);
		middle = (max + min);
		middle /= 2.0f;
		this->size.x = w;
		this->size.y = h;
	}
};
class KCollision
{
public:
	static bool   RectToPoint(KRect rt, int x, int y);
	static bool   RectToPoint(KRect rt, KVector2 v);
	static KCollisionResult    RectToRect(KRect, KRect);
	static KCollisionResult    ToRect(KRect rt1, KRect rt2);
	static KRect  UnionRect(KRect rt1, KRect rt2);
	static bool  IntersectRect(KRect rt1, KRect rt2, KRect* rt);

	static bool   BoxToPoint(KBox rt, int x, int y, int z);
	static bool   BoxToPoint(KBox rt, KVector3 v);
	static KCollisionResult    BoxToBox(KBox, KBox);
	static KBox   UnionBox(KBox rt1, KBox rt2);
	static bool  IntersectBox(KBox rt1, KBox rt2, KBox* rt);
};

