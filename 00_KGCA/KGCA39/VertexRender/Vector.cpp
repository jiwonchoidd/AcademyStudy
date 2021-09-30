#include "Vector.h"
float   Vector2::operator | (Vector2 p)
{
	return x * p.x + y * p.y;
}
float   Vector2::DotProduct(Vector2 a, Vector2 b)
{
	return a.x * b.x + a.y * b.y;
}
float    Vector2::Length()
{
	float ret = x * x + y * y;
	return sqrt(ret);
}
Vector2 Vector2::Normalize()
{
	Vector2 d;
	float length = 1.0f / Length();
	d.x = d.x * length;
	d.y = d.y * length;
	return d;
}
float   Vector2::ProjectionPoint(
	Vector2 q1, Vector2 q2, Vector2 v)
{
	Vector2 a = q2 - q1;
	Vector2 b = v - q1;
	Vector2 p = a * ((b | a) / (a | a));
	Vector2 w = b - p;
	return w.Length();
}