#pragma once
#include "TObject.h"

class THero : public TObject
{
public:
	bool     Init();
	bool     Frame();
	bool     Render();
	bool     Release();
public:
	void	 Left();
	void	 Right();
	void	 Up();
	void	 Down();
public:
	THero();
	virtual ~THero();
};

