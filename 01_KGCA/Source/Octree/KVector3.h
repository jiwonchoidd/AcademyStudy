#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
class KVector3
{
public:
	float x = 0;
	float y = 0;
	float z = 0;

	//연산자 오버로딩
	KVector3 operator+(KVector3 p)
	{
		KVector3 ret;
		ret.x = this->x + p.x;
		ret.y = this->y + p.y;
		ret.z = this->z + p.z;
		return ret;
	}
	KVector3  operator * (float s)
	{
		KVector3 ret;
		ret.x = x * s;
		ret.y = y * s;
		ret.z = z * s;
		return ret;
	}
	KVector3  operator - (KVector3 p)
	{
		KVector3 ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		ret.z = z - p.z;
		return ret;
	}
	KVector3  operator += (KVector3 p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}
	// 내적
	float   operator | (KVector3 p);
	float   KDot(KVector3 a, KVector3 b);
	KVector3 Normalize();
	KVector3 MiddleVector(KVector3 a, KVector3 b);
	float    Length();
	KVector3(){}
	KVector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

