#pragma once
#include "GCore.h"
class Sample : public GCore
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

