#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
struct TVector
{
	float x = 0;
	float y = 0;

	//연산자 오버로딩
	TVector operator+(TVector tv)
	{
		TVector ret;
		ret.x = this->x+tv.x;
		ret.y = this->y + tv.y;
		return ret;
	}
	TVector  operator * (float s)
	{
		TVector ret;
		ret.x = x * s;
		ret.y = y * s;
		return ret;
	}
	TVector  operator - (TVector p)
	{
		TVector ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		return ret;
	}
	// 내적
	float   operator | (TVector p);
	float   DotProduct(TVector a, TVector b);
	float   ProjectionPoint(TVector q1, TVector q2, TVector p);
	TVector Normalize();
	float    Length();
	TVector(){}
	TVector(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

