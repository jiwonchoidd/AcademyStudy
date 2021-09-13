#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct TVector
{
	float x = 0;
	float y = 0;
	//������ �����ε�
	TVector operator+ (TVector p)
	{
		TVector ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		return ret;
	}
	//������
	TVector() {}
	TVector(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
struct Rect
{
	float x, y, w, h;
};

struct TRect
{
	TVector p0;
	TVector p1;
	TVector wh;
};
