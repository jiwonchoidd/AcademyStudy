#include "KVector3.h"
float   KVector3:: operator | (KVector3 p)
{
	//내적 공식
	//a.x * b.x + a.y * b.y 제2 코사인법칙
	return x * p.x + y * p.y + z * p.z;
}
// 내적
float KVector3::KDot(KVector3 a, KVector3 b)
{
	return a.x * b.x + a.y * b.y + a.z + b.z;
}
KVector3 KVector3::Normalize()
{
	//노말라이즈 정규화
	KVector3 d;
	float length = 1.0f / Length();
	d.x = d.x * length;
	d.y = d.y * length;
	d.z = d.z * length;
	return d;
}

KVector3 KVector3::MiddleVector(KVector3 a, KVector3 b)
{
	//( (x1+x2)/2, (y1+y2)/2 )
	KVector3 d;
	d.x = (a.x + b.x) / 2;
	d.y = (a.y + b.y) / 2;
	d.z = (a.z + b.z) / 2;
	return d;
}

float KVector3::Length()
{
	//피타고라스
	float ret = x * x + y * y + z * z;
	return sqrt(ret);
}