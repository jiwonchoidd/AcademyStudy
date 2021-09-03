#pragma once
#include "TCore.h"

class Sample  : public TCore
{
public:
	bool		Init();
	bool		Release();
public:
	Sample(void);
	~Sample(void);
};
