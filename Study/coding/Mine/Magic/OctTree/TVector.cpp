#include "TVector.h"
float   TVector:: operator | (TVector p)
{
	//���� ����
	//a.x * b.x + a.y * b.y ��2 �ڻ��ι�Ģ
	return x * p.x + y * p.y;
}
float TVector::DotProduct(TVector a, TVector b)
{
	return a.x * b.x + a.y * b.y;
}
float TVector::ProjectionPoint(TVector q1, TVector q2, TVector v)
{
	//xx���� ���� xy���� ���ϱ� x
	TVector a = q2 - q1;
	TVector b = v - q1;
	TVector p = a * ((b | a) / (a | a));
	TVector w = b - p;
	return w.Length();
}
TVector TVector::Normalize()
{
	//���ؾȰ�
	TVector d;
	float length = 1.0f / Length();
	d.x = d.x * length;
	d.y = d.y * length;
	return d;
}
float TVector::Length()
{
	//��Ÿ���
	float ret = x * x + y * y;
	return sqrt(ret);
}