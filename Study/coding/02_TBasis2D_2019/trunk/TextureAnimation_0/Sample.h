#pragma once
#pragma comment(lib,"TCoreDx_0.lib")
#include "TCoreDx.h"
#include "TSprite.h"
//#define USED_ONLY_VERTEX
#define MAX_OBJECT 3

class Sample : public TCoreDx
{
	TSprite	m_Object[MAX_OBJECT];	
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	Sample();
	virtual ~Sample();
};

