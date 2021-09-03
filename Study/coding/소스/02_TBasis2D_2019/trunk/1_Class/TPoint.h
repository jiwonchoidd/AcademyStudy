#pragma once
#include <iostream>
using namespace std;
class TPoint
{
public:
	int x, y;
	void SetPosition(int _x, int _y);
	void Move(int _x, int _y);
	void Show(void);
public:
	TPoint();
	~TPoint();
};

