#pragma once
#pragma comment(lib,"TCoreDx_1.lib")
#include "TCoreDx.h"
#include "THeightMap.h"
class Sample : public TCoreDx
{
public:
	THeightMap		m_Map;
	D3DXMATRIX		m_matWorldMap;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	Sample();
	virtual ~Sample();
};

