#pragma once
#include <math.h>
struct Vector2
{
	float x;
	float y;
	Vector2  operator += (Vector2 p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}
	Vector2  operator * (float s)
	{
		Vector2 ret;
		ret.x = x * s;
		ret.y = y * s;
		return ret;
	}
	Vector2  operator / (float fValue)
	{
		Vector2 ret;
		ret.x = x / fValue;
		ret.y = y / fValue;
		return ret;
	}
	Vector2  operator + (Vector2 p)
	{
		Vector2 ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		return ret;
	}
	Vector2  operator - (Vector2 p)
	{
		Vector2 ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		return ret;
	}
	// ³»Àû
	float   operator | (Vector2 p);
	float   DotProduct(Vector2 a, Vector2 b);
	float   ProjectionPoint(Vector2 q1, Vector2 q2, Vector2 p);
	Vector2 Normalize();
	float    Length();
	//bool     PointToRect(Rect desk) { return true; };
public:
	Vector2() {}
	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
class Vector3
{
public:
	float x;
	float y;
	float z;
	Vector3  operator += (Vector3 p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}
	Vector3  operator * (float s)
	{
		Vector3 ret;
		ret.x = x * s;
		ret.y = y * s;
		ret.z = z * s;
		return ret;
	}
	Vector3  operator + (Vector3 p)
	{
		Vector3 ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		ret.z = z + p.z;
		return ret;
	}
	Vector3  operator - (Vector3 p)
	{
		Vector3 ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		ret.z = z - p.z;
		return ret;
	}
	Vector3  operator / (float fValue)
	{
		Vector3 ret;
		ret.x = x / fValue;
		ret.y = y / fValue;
		ret.z = z / fValue;
		return ret;
	}
public:
	Vector3() {}
	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
	Vector4() {}
	Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};