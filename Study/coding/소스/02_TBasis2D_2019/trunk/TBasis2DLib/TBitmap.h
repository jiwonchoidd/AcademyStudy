#pragma once
#include "TStd.h"
#include <windows.h>
#include <wincodec.h>
#include <windowsx.h>
#include <commdlg.h>

#pragma comment( lib, "Msimg32.lib")
#pragma comment( lib, "windowscodecs.lib")

//dds format -> DXGI_FORMAT_BC1_UNORM, DXGI_FORMAT_BC2_UNORM 및 DXGI_FORMAT_BC3_UNORM 형식으로 인코딩 된 DDS 이미지를 지원
//jpg, png, gif, tif, bmp 지원, tga format는 다른 포멧을 변환해서 사용해야 함.

class TBitmap
{
public:
	HBITMAP m_hBitmap;
	HDC		m_hMemDC;
	LPBYTE	m_pRaster;
	BITMAP  m_BitmapInfo;
	BLENDFUNCTION m_BlendFunction;
public:
	bool  Init();
	bool  Frame();
	bool  Render();
	bool  Release();
public:
	HBITMAP  LoadDIB(wstring filename);
	bool  CreateDIB(LONG iWidth, LONG iHeight, int iBitCount =24);
	bool  Load(wstring filename);

	bool  Draw(RECT desk, RECT src, DWORD op=SRCCOPY, int iType=0);
	bool  DrawColorKey(LONG x, LONG Y, DWORD color);
	bool  DrawColorKey(	RECT desk,
						RECT src, DWORD color=RGB(255,0,255));
	bool  DrawAlphaBlend(RECT desk, RECT src,BYTE alpha = 255);
	bool  DrawAlphaBlend(RECT desk, RECT src,BLENDFUNCTION bf);
	bool  DrawAlphaBlend(LONG x, LONG y, BYTE alpha = 255);
	bool  DrawAlphaBlend(LONG x, LONG y, BYTE alpha, BLENDFUNCTION bf);
public:
	////windowscodecs.lib;
	//IWICImagingFactory *m_pIWICFactory;
	IWICBitmapSource *m_pOriginalBitmapSource;

	HRESULT CreateDIBFromFile(T_STR szFileName);
	BOOL LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cbFileName);
	HRESULT ConvertBitmapSource(HWND hWnd, IWICBitmapSource **ppToRenderBitmapSource);
	HRESULT CreateDIBSectionFromBitmapSource(IWICBitmapSource *pToRenderBitmapSource);
public:
	TBitmap();
	virtual ~TBitmap();
};

