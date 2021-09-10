#pragma once
#include "TCore.h"
#include "TServer.h"

class Sample  : public TCore
{
public:
	bool		Init();
	bool		Frame();
	bool		Release();
public:
	Sample(void);
	~Sample(void);
};