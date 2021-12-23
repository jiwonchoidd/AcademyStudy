#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
struct KVector
{
	float x = 0;
	float y = 0;
	float z = 0;
	//연산자 오버로딩
	KVector operator+(KVector tv)
	{
		KVector ret;
		ret.x = this->x + tv.x;
		ret.y = this->y + tv.y;
		ret.z = this->z + tv.z;
		return ret;
	}
	KVector  operator * (float s)
	{
		KVector ret;
		ret.x = x * s;
		ret.y = y * s;
		ret.z = z * s;
		return ret;
	}
	KVector  operator - (KVector p)
	{
		KVector ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		ret.z = z - p.z;
		return ret;
	}
	KVector  operator += (KVector p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}
	// 내적
	float   operator | (KVector p);
	float   DotProduct(KVector a, KVector b);
	float   ProjectionPoint(KVector q1, KVector q2, KVector p);
	KVector Normalize();
	float    Length();
	KVector(){}
	KVector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

