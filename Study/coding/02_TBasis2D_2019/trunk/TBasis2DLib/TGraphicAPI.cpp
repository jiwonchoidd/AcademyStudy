#include "TGraphicAPI.h"
namespace TBASIS_CORE_LIB
{
	HDC		g_hScreenDC;
	HDC		g_hOffScreenDC;
}
IWICImagingFactory * TGraphicAPI::m_pIWICFactory = nullptr;
bool	TGraphicAPI::Init() 
{
	HRESULT hr = S_OK;
	//hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr)) return false;
	// Create WIC factory
	hr = CoCreateInstance(CLSID_WICImagingFactory,NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&m_pIWICFactory));
	if (FAILED(hr)) return false;

	m_hScreenDC = GetDC(g_hWnd);
	g_hScreenDC = m_hScreenDC;
	m_hOffScreenDC = CreateCompatibleDC(m_hScreenDC);
	m_hOffScreenBitmap = CreateCompatibleBitmap(
		m_hScreenDC, g_rtClient.right, g_rtClient.bottom);
	SelectObject(m_hOffScreenDC, m_hOffScreenBitmap);
	g_hOffScreenDC = m_hOffScreenDC;

	COLORREF bkColor = RGB(128, 128, 128);
	m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hOffScreenDC, m_hbrRed);
	return true;
};
bool	TGraphicAPI::Frame() {
	return true;
};
bool	TGraphicAPI::PreRender() 
{
	PatBlt( m_hOffScreenDC, 0, 0, 
			g_rtClient.right, g_rtClient.bottom, PATCOPY);
	return true;
};
bool	TGraphicAPI::Render()
{	
	return true;
};
bool	TGraphicAPI::PostRender()
{
	BitBlt(m_hScreenDC, 0, 0, g_rtClient.right, g_rtClient.bottom,
		   m_hOffScreenDC, 0, 0, SRCCOPY);
	return true;
};
bool	TGraphicAPI::Release() 
{
	DeleteObject(m_hbrRed);
	DeleteObject(m_hOffScreenBitmap);
	ReleaseDC(g_hWnd, m_hOffScreenDC);
	ReleaseDC(g_hWnd, m_hScreenDC);
	SafeRelease(m_pIWICFactory);
	return true;
};

BOOL TGraphicAPI::LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cchFileName)
{
	pszFileName[0] = L'\0';

	OPENFILENAME ofn;
	RtlZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"All Image Files\0"              L"*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico\0"
		L"Windows Bitmap\0"               L"*.bmp;*.dib\0"
		L"High Definition Photo\0"        L"*.wdp;*.mdp;*.hdp\0"
		L"Graphics Interchange Format\0"  L"*.gif\0"
		L"Portable Network Graphics\0"    L"*.png\0"
		L"JPEG File Interchange Format\0" L"*.jpg;*.jpeg\0"
		L"Tiff File\0"                    L"*.tif\0"
		L"Icon\0"                         L"*.ico\0"
		L"All Files\0"                    L"*.*\0"
		L"\0";
	ofn.lpstrFile = pszFileName;
	ofn.nMaxFile = cchFileName;
	ofn.lpstrTitle = L"Open Image";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	// Display the Open dialog box. 
	return GetOpenFileName(&ofn);
}
