#pragma once
#include "TCore.h"
class Sample : public TCore
{
public:
	bool		Init();
	bool		Frame();
	bool		Render();
public:
	Sample();
	virtual ~Sample();
};

