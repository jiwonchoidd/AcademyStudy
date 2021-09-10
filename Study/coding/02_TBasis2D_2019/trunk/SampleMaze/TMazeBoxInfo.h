#pragma once
#include "TStd.h"
#include <bitset>

struct MazeBoxInfo
{
	unsigned left	: 1;
	unsigned right	: 1;
	unsigned top	: 1;
	unsigned bottom : 1;

	MazeBoxInfo()
	{
		left	= 1;
		right	= 1;
		top		= 1;
		bottom	= 1;
	}
	void SetTop(int value) { top = value; }
	void SetBottom(int value) { bottom = value; }
	void SetLeft(int value) { left = value; }
	void setright(int value) { right = value; }
	int GetRight() { return right; }
	int getLeft() { return left; }
	int GetTop() { return top; }
	int GetBottom() { return bottom; }
};

class TMazeBoxInfo
{
public:
	TMazeBoxInfo();
	virtual ~TMazeBoxInfo();
};

