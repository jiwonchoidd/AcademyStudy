#include "KVector.h"
float   KVector:: operator | (KVector p)
{
	//���� ����
	//a.x * b.x + a.y * b.y ��2 �ڻ��ι�Ģ
	return x * p.x + y * p.y + z * p.z;
}
float KVector::DotProduct(KVector a, KVector b)
{
	return a.x * b.x + a.y * b.y + a.z + b.z;
}
float KVector::ProjectionPoint(KVector q1, KVector q2, KVector v)
{
	//xx���� ���� xy���� ���ϱ� x
	KVector a = q2 - q1;
	KVector b = v - q1;
	KVector p = a * ((b | a) / (a | a));
	KVector w = b - p;
	return w.Length();
}
KVector KVector::Normalize()
{
	//�븻������ ����ȭ
	KVector d;
	float length = 1.0f / Length();
	d.x = d.x * length;
	d.y = d.y * length;
	d.z = d.z * length;
	return d;
}
float KVector::Length()
{
	//��Ÿ���
	float ret = x * x + y * y + z * z;
	return sqrt(ret);
}