#pragma once
#pragma comment(lib,"TCoreDx_1.lib")
#include "TCoreDx.h"
#include "THeightMap.h"

class Sample : public TCoreDx
{
public:
	THeightMap		m_Map;
	D3DXMATRIX		m_matWorldMap;
	TLineShape		m_Line;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	void    DrawBoundingBox(T_Box box, D3DXVECTOR4  vColor);
public:
	Sample();
	virtual ~Sample();
};

