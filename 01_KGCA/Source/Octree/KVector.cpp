#include "KVector.h"
float   KVector:: operator | (KVector p)
{
	//내적 공식
	//a.x * b.x + a.y * b.y 제2 코사인법칙
	return x * p.x + y * p.y + z * p.z;
}
float KVector::DotProduct(KVector a, KVector b)
{
	return a.x * b.x + a.y * b.y + a.z + b.z;
}
float KVector::ProjectionPoint(KVector q1, KVector q2, KVector v)
{
	//xx내적 분의 xy내적 곱하기 x
	KVector a = q2 - q1;
	KVector b = v - q1;
	KVector p = a * ((b | a) / (a | a));
	KVector w = b - p;
	return w.Length();
}
KVector KVector::Normalize()
{
	//노말라이즈 정규화
	KVector d;
	float length = 1.0f / Length();
	d.x = d.x * length;
	d.y = d.y * length;
	d.z = d.z * length;
	return d;
}
float KVector::Length()
{
	//피타고라스
	float ret = x * x + y * y + z * z;
	return sqrt(ret);
}