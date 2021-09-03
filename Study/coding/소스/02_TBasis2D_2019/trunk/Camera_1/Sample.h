#pragma once
#pragma comment(lib,"TCoreDx_0.lib")
#include "TCoreDx.h"
#include "TSprite.h"
#include "TDebugCamera.h"
#include "TShape.h"
//#define USED_ONLY_VERTEX

#define MAX_OBJECT 3
class Sample : public TCoreDx
{
	TSprite				m_Object[MAX_OBJECT];	
	ID3D11Buffer*		m_pcbVSPerObject;
	CB_VS_PER_OBJECT    m_cbVsPerObject;
	TDebugCamera		m_Camera;
	TShape*				m_pBoxObj[3];
	TShape*				m_pLineObj;
	D3DXMATRIX			m_matWorldLine;
	D3DXMATRIX			m_matWorldBox[3];
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	//LRESULT MsgProc(HWND hWnd,
	//	UINT msg,
	//	WPARAM wParam,
	//	LPARAM lParam)
	//{
	//	return m_Input.MsgProc( hWnd, msg, wParam, lParam);
	//};
public:
	Sample();
	virtual ~Sample();
};

