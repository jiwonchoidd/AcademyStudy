#include "TWrite.h"

bool TWrite::Reset(IDXGISurface1*  pSurface)
{
	HRESULT hr;
	// DPI
	SAFE_RELEASE(m_pD2DRT);
	float dpiX, dpiY;
	m_pD2DF->GetDesktopDpi(&dpiX, &dpiY);
	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));
	props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	props.dpiX = dpiX;
	props.dpiY = dpiY;
	if (FAILED(hr=m_pD2DF->CreateDxgiSurfaceRenderTarget(	pSurface,
		(const D2D1_RENDER_TARGET_PROPERTIES*)&props, &m_pD2DRT)))	
	{
		return false;
	}
	return true;
}
bool		TWrite::Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1*  pSurface)
{
	HRESULT hr;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DF);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,	__uuidof(IDWriteFactory),(IUnknown**)&m_pWF);
	m_pWF->CreateTextFormat(L"±Ã¼­", NULL,
		DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,
		24,	//L"en-us", // 
		L"ko-kr",	&m_pWTF);

	// DPI
	float dpiX, dpiY;
	m_pD2DF->GetDesktopDpi(&dpiX, &dpiY);
	D2D1_RENDER_TARGET_PROPERTIES props;
	ZeroMemory(&props, sizeof(props));
	props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	props.dpiX = dpiX;
	props.dpiY = dpiY;
	if (FAILED(hr=m_pD2DF->CreateDxgiSurfaceRenderTarget(pSurface,
			(const D2D1_RENDER_TARGET_PROPERTIES*)&props, &m_pD2DRT)))
	{
		return false;
	}
	D3DCOLORVALUE color = { 1.0f,0.0f,0.0f,1.0f };
	m_pD2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pBrush);
	return true;
}
bool		TWrite::Init() {
	return true;
}
bool		TWrite::Frame() {
	return true;
}
bool		TWrite::PreRender()
{
	if (m_pD2DRT != NULL)m_pD2DRT->BeginDraw();
	return true;
}
bool		TWrite::PostRender()
{
	if (m_pD2DRT != NULL)	m_pD2DRT->EndDraw();
	return true;
}
bool		TWrite::Draw(D2D_RECT_F& rc, TCHAR* pText,D2D1::ColorF color)
{
	if (m_pD2DRT == NULL) return false;
	m_pBrush->SetColor(color);
	m_pD2DRT->DrawTextW(pText, wcslen(pText),m_pWTF, rc, m_pBrush);
	return true;
}
bool		TWrite::Render()
{
	return true;
}
bool		TWrite::Release() 
{
	SAFE_RELEASE(m_pBrush);
	SAFE_RELEASE(m_pWTF);	
	SAFE_RELEASE(m_pD2DRT);
	SAFE_RELEASE(m_pWF);
	SAFE_RELEASE(m_pD2DF);	
	return true;
}

TWrite::TWrite()
{
}


TWrite::~TWrite()
{
}
