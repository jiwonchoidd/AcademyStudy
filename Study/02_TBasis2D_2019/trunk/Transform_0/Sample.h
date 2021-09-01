#pragma once
#pragma comment(lib,"TCoreDx_0.lib")
#include "TCoreDx.h"
#include "TSprite.h"

//#define USED_ONLY_VERTEX
#define MAX_OBJECT 3
struct CB_VS_PER_OBJECT
{
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
};
class Sample : public TCoreDx
{
	TSprite				m_Object[MAX_OBJECT];	
	ID3D11Buffer*		m_pcbVSPerObject;
	CB_VS_PER_OBJECT    m_cbVsPerObject;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	Sample();
	virtual ~Sample();
};

