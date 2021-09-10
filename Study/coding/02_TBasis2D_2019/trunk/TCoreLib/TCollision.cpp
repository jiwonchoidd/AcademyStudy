#include "TCollision.h"

bool TCollision::RectInPt(RECT rt, POINT pt)
{
	if (rt.left <= pt.x && rt.right >= pt.x)
	{
		if (rt.top <= pt.y && rt.bottom >= pt.y)
		{
			return true;
		}
	}
	return false;
}
bool  TCollision::RectInRect(RECT rt1, RECT rt2)
{
	POINT rtCenter1;
	POINT rtCenter2;
	rtCenter1.x = (rt1.right + rt1.left) / 2;
	rtCenter1.y = (rt1.bottom + rt1.top) / 2;
	rtCenter2.x = (rt2.right + rt2.left) / 2;
	rtCenter2.y = (rt2.bottom + rt2.top) / 2;

	POINT rtRadius1, rtRadius2;
	rtRadius1.x = rt1.right - rtCenter1.x;
	rtRadius1.y = rt1.bottom - rtCenter1.y;
	rtRadius2.x = rt2.right - rtCenter2.x;
	rtRadius2.y = rt2.bottom - rtCenter2.y;
	POINT rtDistance;
	rtDistance.x = abs(rtCenter2.x - rtCenter1.x);
	rtDistance.y = abs(rtCenter2.y - rtCenter1.y);

	if (rtDistance.x <= (rtRadius1.x + rtRadius2.x) &&
		rtDistance.y <= (rtRadius1.y + rtRadius2.y))
	{
		return true;
	}
	return false;
}
bool TCollision::SphereInSphere(RECT rt1, RECT rt2)
{
	POINT c0, c1;
	double r0, r1, fDistance;
	c0.x = (rt1.right + rt1.left) / 2;
	c0.y = (rt1.bottom + rt1.top) / 2;
	c1.x = (rt2.right + rt2.left) / 2;
	c1.y = (rt2.bottom + rt2.top) / 2;

	fDistance = sqrt(
		(c1.x - c0.x) * (c1.x - c0.x)
		+
		(c1.y - c0.y) * (c1.y - c0.y));


	r0 = sqrt(
		(rt1.right - rt1.left) * (rt1.right - rt1.left)
		+
		(rt1.bottom - rt1.top) * (rt1.bottom - rt1.top)) / 2;
	r1 = sqrt(
		(rt2.right - rt2.left) * (rt2.right - rt2.left)
		+
		(rt2.bottom - rt2.top) * (rt2.bottom - rt2.top)) / 2;

	if ((r0 + r1) > fDistance)
	{
		return true;
	}
	return false;
}
bool TCollision::SphereInPoint(TSphere rt, POINT pos)
{
	float fDistance = sqrt(
		(rt.vCenter.x - pos.x) * (rt.vCenter.x - pos.x)
		+
		(rt.vCenter.y - pos.y) * (rt.vCenter.y - pos.y));
	if (fDistance < rt.fRadius)
	{
		return true;
	}
	return false;
}
bool TCollision::SphereInPoint(TSphere rt, TPoint pos)
{
	float fDistance = sqrt(
		(rt.vCenter.x - pos.x) * (rt.vCenter.x - pos.x)
		+
		(rt.vCenter.y - pos.y) * (rt.vCenter.y - pos.y));
	if (fDistance < rt.fRadius)
	{
		return true;
	}
	return false;
}
TCollision::TCollision()
{
}


TCollision::~TCollision()
{
}
