#pragma once
#include "Core.h"
class Sample : public Core
{
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};

