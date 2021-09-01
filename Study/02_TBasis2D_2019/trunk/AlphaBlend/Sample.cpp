#pragma once
#include "TCore.h"
#include "TBitmap.h"

class Sample : public TCore
{
	HBITMAP		m_hBitmap;
	HDC			m_hMemDC;
	HBITMAP		m_hOldBitmap;
	BITMAP		m_BmpInfo;
public:
	bool		Init();
	bool		Load(TCHAR* pszLoadFile);
	bool		Render();
	bool		Release();
public:
	void DrawAlphaBlend(HWND hWnd, HDC hdcwnd);
	bool AlphaBlendU(	HDC dcDest, int x, int y, int cx, int cy,
						HDC dcSrc, int sx, int sy, int scx, int scy, 
						float alpha);
	bool AlphaBlendCK(	HDC dcDest, int x, int y, int cx, int cy,
						HDC dcSrc, int sx, int sy, int scx, int scy,
						float alpha,
						COLORREF rgbMask);

public:
	Sample() {};
	virtual ~Sample() {};
};
bool Sample::Init()
{
	Load(L"../../data/kg.bmp");
	return true;
}
bool Sample::Render()
{
	static float fValue = 0;
	fValue += g_fSecPerFrame * 50;
	if (fValue > 255) fValue = 0.0f;
	DrawAlphaBlend(m_hWnd, m_hScreenDC);
	AlphaBlendU(m_hOffScreenDC, 300, 100, m_BmpInfo.bmWidth, m_BmpInfo.bmHeight,
		m_hMemDC, 0, 0, m_BmpInfo.bmWidth, m_BmpInfo.bmHeight, fValue/255.0f);
	AlphaBlendCK(	m_hOffScreenDC, 300, 300, m_BmpInfo.bmWidth, m_BmpInfo.bmHeight,
		m_hMemDC, 0, 0, m_BmpInfo.bmWidth, m_BmpInfo.bmHeight, fValue/255.0f, RGB(255, 255, 255));
	return true;
}
bool Sample::Release()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
}
bool Sample::Load(TCHAR* pszLoadFile)
{
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		pszLoadFile, IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE |
		LR_LOADFROMFILE);
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	// 생성된 메모리에 비트맵 적용
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};

//In the top area, constant alpha = 50 % but there is no source alpha.
//In the middle area, constant alpha = 100 % (disabled) and source alpha is 0 (transparent)in the middle of the bitmap and 0xff (opaque)elsewhere.
//In the bottom area, constant alpha = 75 % and source alpha changes.

void Sample::DrawAlphaBlend(HWND hWnd, HDC hdcwnd)
{
	HDC hdc;               // handle of the DC we will create  
	BLENDFUNCTION bf;      // structure for alpha blending 
	HBITMAP hbitmap;       // bitmap handle 
	BITMAPINFO bmi;        // bitmap header 
	VOID *pvBits;          // pointer to DIB section 
	ULONG   ulWindowWidth, ulWindowHeight;      // window width/height 
	ULONG   ulBitmapWidth, ulBitmapHeight;      // bitmap width/height 
	RECT    rt;            // used for getting window dimensions 
	UINT32   x, y;          // stepping variables 
	UCHAR ubAlpha;         // used for doing transparent gradient 
	UCHAR ubRed;
	UCHAR ubGreen;
	UCHAR ubBlue;
	float fAlphaFactor;    // used to do premultiply 

						   // get window dimensions 
	GetClientRect(hWnd, &rt);

	// calculate window width/height 
	ulWindowWidth = rt.right - rt.left;
	ulWindowHeight = rt.bottom - rt.top;

	// make sure we have at least some window size 
	if ((!ulWindowWidth) || (!ulWindowHeight))
		return;

	// divide the window into 3 horizontal areas 
	ulWindowHeight = ulWindowHeight / 3;

	// create a DC for our bitmap -- the source DC for AlphaBlend  
	hdc = CreateCompatibleDC(NULL);
	if (hdc == NULL)  return;

	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = ulBitmapWidth = ulWindowWidth - (ulWindowWidth / 5) * 2;
	bmi.bmiHeader.biHeight = ulBitmapHeight = ulWindowHeight - (ulWindowHeight / 5) * 2;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;         // four 8-bit components 
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

	////////////////////////////////////////// (1) 픽쉘에 직접 접근하여 세팅 ///////////////////////////////////////////////////	
	hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
	SelectObject(hdc, hbitmap);
	for (y = 0; y < ulBitmapHeight; y++)
	{
		for (x = 0; x < ulBitmapWidth; x++)
		{
			((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x000000ff; // 0xaarrggbb
		}
	}
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha =  0x7f;  // 0x7f=128,m 0xff = 0% transparency 
	bf.AlphaFormat = 0;              // 알파채널 무시
	if (!AlphaBlend(m_hOffScreenDC, ulWindowWidth / 5, ulWindowHeight / 5,
		ulBitmapWidth, ulBitmapHeight,
		hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
	{
		return;                    
	}

	//////////////////////////////////////////////// (2)중간 영역에서 constant alpha = 100% (disabled), /////////////////////////////////////////
	// 소스알파 및 알파 상수 사요함
	//// 알파는 중간에만 0으로 투명이며 나머지는 불투명처리  
	for (y = 0; y < ulBitmapHeight; y++)
	{
		for (x = 0; x < ulBitmapWidth; x++)
		{
			if ((x > (int)(ulBitmapWidth / 5)) && (x < (ulBitmapWidth - ulBitmapWidth / 5)) &&
				(y > (int)(ulBitmapHeight / 5)) && (y < (ulBitmapHeight - ulBitmapHeight / 5)))
				//in middle of bitmap: source alpha = 0 (transparent). 
				// This means multiply each color component by 0x00. 
				// Thus, after AlphaBlend, we have a, 0x00 * r,  
				// 0x00 * g,and 0x00 * b (which is 0x00000000) 
				// for now, set all pixels to red 
				((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0x00ff0000;
			else
				// in the rest of bitmap, source alpha = 0xff (opaque)  
				// and set all pixels to blue  
				((UINT32 *)pvBits)[x + y * ulBitmapWidth] = 0xff0000ff;
		}
	}
	//endif;


	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha  
	bf.SourceConstantAlpha = 0xff;  // opaque (disable constant alpha) 

	if (!AlphaBlend(m_hOffScreenDC, ulWindowWidth / 5, ulWindowHeight / 5 + ulWindowHeight, ulBitmapWidth, ulBitmapHeight, hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf))
		return;


	/////////////////////////////////////////////////(2)소스알파 사용 드로우 //////////////////////////////////////////////////
	// bottom window area, use constant alpha = 75% and a changing 
	// source alpha. Create a gradient effect using source alpha, and  
	// then fade it even more with constant alpha 
	ubRed = 0x00;
	ubGreen = 0x00;
	ubBlue = 0xff;

	for (y = 0; y < ulBitmapHeight; y++)
	{
		for (x = 0; x < ulBitmapWidth; x++)
		{
			// for a simple gradient, base the alpha value on the x  
			// value of the pixel  
			ubAlpha = (UCHAR)((float)x / (float)ulBitmapWidth * 255);
			//calculate the factor by which we multiply each component 
			fAlphaFactor = (float)ubAlpha / (float)0xff;
			// multiply each pixel by fAlphaFactor, so each component  
			// is less than or equal to the alpha value. 
			((UINT32 *)pvBits)[x + y * ulBitmapWidth]
				= (ubAlpha << 24) |                       //0xaa000000 
				((UCHAR)(ubRed * fAlphaFactor) << 16) |  //0x00rr0000 
				((UCHAR)(ubGreen * fAlphaFactor) << 8) | //0x0000gg00 
				((UCHAR)(ubBlue   * fAlphaFactor));      //0x000000bb 
		}
	}

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;   // use source alpha  
	bf.SourceConstantAlpha = 0xbf;   // 75% opaqueness 

	AlphaBlend(m_hOffScreenDC, ulWindowWidth / 5,
		ulWindowHeight / 5 + 2 * ulWindowHeight, ulBitmapWidth,
		ulBitmapHeight, hdc, 0, 0, ulBitmapWidth,
		ulBitmapHeight, bf);

	DeleteObject(hbitmap);
	DeleteDC(hdc);

}

// 전체 픽쉘에 소스의 알파값으로 배경과 블랜딩함
bool Sample::AlphaBlendU(	HDC dcDest, int x, int y, int cx, int cy,
							HDC dcSrc, int sx, int sy, int scx, int scy, 
	float alpha)
{
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression
	BMI.biSizeImage = 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed = 0;           // Always use the whole palette.
	BMI.biClrImportant = 0;

	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	hbmSrc = CreateDIBSection(dcSrc, (BITMAPINFO *)&BMI,DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	hbmDest = CreateDIBSection(dcDest, (BITMAPINFO *)&BMI,DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);

	HDC dc = CreateCompatibleDC(NULL);
	HBITMAP dcOld = (HBITMAP)SelectObject(dc, hbmSrc);

	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy,scx, scy, SRCCOPY))
		return false;

	SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY))
		return false;

	SelectObject(dc, dcOld);
	DeleteDC(dc);

	for (int j = 0; j < cy; ++j)
	{
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];

		for (int i = 0; i < cx; ++i)
		{
			pbSrcRGB[0] = (pbDestRGB[0] * (1 - alpha) + pbSrcRGB[0] * alpha);// >> 8;
			pbSrcRGB[1] = (pbDestRGB[1] * (1 - alpha) + pbSrcRGB[1] * alpha);// >> 8;
			pbSrcRGB[2] = (pbDestRGB[2] * (1 - alpha) + pbSrcRGB[2] * alpha);// >> 8;
			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = CreateCompatibleDC(NULL);

	dcOld = (HBITMAP)SelectObject(dc, hbmSrc);
	if (!BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, SRCCOPY))
		return false;
	DeleteDC(dc);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);

	return true;
}
// rgbMask 픽쉘을 제외한 픽쉘에 알파소스의 알파값으로 배경과 블랜딩함.
bool Sample::AlphaBlendCK(	HDC dcDest, int x, int y, int cx, int cy,
							HDC dcSrc, int sx, int sy, int scx, int scy,
							float alpha,
							COLORREF rgbMask)
{
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression
	BMI.biSizeImage = 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed = 0;           // Always use the whole palette.
	BMI.biClrImportant = 0;

	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	hbmSrc = CreateDIBSection(dcSrc, (BITMAPINFO *)&BMI,DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	hbmDest = CreateDIBSection(dcDest, (BITMAPINFO *)&BMI,	DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);

	HDC dc = CreateCompatibleDC(NULL);
	HBITMAP dcOld = (HBITMAP)SelectObject(dc, hbmSrc);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY))
	{
		return false;
	}
	SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, x, cx, cy, SRCCOPY))
	{
		return false;
	}
	SelectObject(dc, dcOld);
	DeleteDC(dc);

	int red = GetRValue(rgbMask);
	int green = GetGValue(rgbMask);
	int blue = GetBValue(rgbMask);

	for (int j = 0; j < cy; ++j)
	{
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];

		for (int i = 0; i < cx; ++i)
		{
			if (pbSrcRGB[0] != blue ||
				pbSrcRGB[1] != green ||
				pbSrcRGB[2] != red)
			{
				pbSrcRGB[0] = (pbDestRGB[0] * (1 - alpha) + pbSrcRGB[0] * alpha);// >> 8;
				pbSrcRGB[1] = (pbDestRGB[1] * (1 - alpha) + pbSrcRGB[1] * alpha);// >> 8;
				pbSrcRGB[2] = (pbDestRGB[2] * (1 - alpha) + pbSrcRGB[2] * alpha);// >> 8;
			}
			else
			{
				pbSrcRGB[0] = pbDestRGB[0];
				pbSrcRGB[1] = pbDestRGB[1];
				pbSrcRGB[2] = pbDestRGB[2];
			}
			
			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = CreateCompatibleDC(NULL);

	dcOld = (HBITMAP)SelectObject(dc, hbmSrc);
	if (!BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, SRCCOPY))
		return false;
	DeleteDC(dc);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);

	return true;
}


//TCORE_RUN(SampleLib_0, 1024, 768);
TCORE_START;
// 윈도우를 생성한다.
if (sample.SetWindow(hInstance,
	L"SampleBitmap_0", 800, 600));// , WS_POPUPWINDOW))
{
	// 생성된 윈도우를 가동한다.
	sample.Run();
}
TCORE_END;