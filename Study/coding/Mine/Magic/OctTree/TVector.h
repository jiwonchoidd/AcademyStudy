#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
struct TVector
{
	float x = 0;
	float y = 0;

	//연산자 오버로딩
	TVector operator+(TVector tv)
	{
		TVector ret;
		ret.x = this->x+tv.x;
		ret.y = this->y + tv.y;
		return ret;
	}

	TVector(){}
	TVector(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

