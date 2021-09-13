#include "TVector.h"
float   TVector:: operator | (TVector p)
{
	//내적 공식
	//a.x * b.x + a.y * b.y 제2 코사인법칙
	return x * p.x + y * p.y;
}
float TVector::DotProduct(TVector a, TVector b)
{
	return a.x * b.x + a.y * b.y;
}
float TVector::ProjectionPoint(TVector q1, TVector q2, TVector v)
{
	//xx내적 분의 xy내적 곱하기 x
	TVector a = q2 - q1;
	TVector b = v - q1;
	TVector p = a * ((b | a) / (a | a));
	TVector w = b - p;
	return w.Length();
}
TVector TVector::Normalize()
{
	//이해안감
	TVector d;
	float length = 1.0f / Length();
	d.x = d.x * length;
	d.y = d.y * length;
	return d;
}
float TVector::Length()
{
	//피타고라스
	float ret = x * x + y * y;
	return sqrt(ret);
}