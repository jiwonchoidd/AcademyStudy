#pragma once
#include <Windows.h>
#include <math.h>

#define TBASIS_EPSILON		((FLOAT)  0.001f)
#define TBASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (TBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / TBASIS_PI))
#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

struct TPoint
{
	float x, y;
	bool operator == (const TPoint& pos)
	{
		return (x == pos.x && y == pos.y);
	}
	TPoint(float fX, float fY)
	{
		x = fX;
		y = fY;
	}
	TPoint()
	{
		x = y = 0.0f;
	}
};

namespace TBASIS_VECTOR {

	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};
	struct float3
	{
		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};
	class Vector2 : public float2
	{
	public:
		Vector2();
		Vector2(const Vector2& v0);
		Vector2(float fX, float fY);
		Vector2(TPoint p);

		// 연산자 재정의 
		Vector2  operator-();
		Vector2 & operator++();
		const Vector2  operator++(int);

		Vector2 operator + (Vector2 const &v0);
		Vector2 operator - (Vector2 const &v0);
		Vector2 operator * (float const &fScala);
		Vector2 operator + (float const &fScala);
		Vector2 operator / (float const &fScala);
		// Dot Product
		float   operator | (Vector2 const &v0);
		Vector2 operator ^ (Vector2 const &v0);
		bool	operator == (Vector2 const &v0);
		// 제곱
		float LengthSquared();
		// 원점으로 부터의 거리
		float Length();
		Vector2 Normal();
		float Angle(Vector2& v0);
		bool  CCW(Vector2 vLook, Vector2 vDir);
	};
}
using namespace TBASIS_VECTOR;