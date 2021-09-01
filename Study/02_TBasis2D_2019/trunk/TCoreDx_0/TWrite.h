#pragma once
#include "TDxStd.h"
#include "d2d1.h"
#include "d2d1helper.h"
#include "dwrite.h"

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
// D3D11_CREATE_DEVICE_BGRA_SUPPORT 디바이스생성시 추가
class TWrite
{
public:
	ID2D1Factory*		m_pD2DF;
	ID2D1RenderTarget*	m_pD2DRT;

	IDWriteFactory*		m_pWF;
	IDWriteTextFormat*	m_pWTF;
	ID2D1SolidColorBrush* m_pBrush;
public:
	bool		Set(HWND hWnd, int iWidth, int iHeight,
		IDXGISurface1*  pSurface);
	bool		Draw(D2D_RECT_F& rc, TCHAR* pText,
		D2D1::ColorF color = D2D1::ColorF(1, 1, 1, 1));
	bool		PreRender();
	bool		PostRender();
	bool		Reset(IDXGISurface1*  pSurface);
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	TWrite();
	~TWrite();
};

