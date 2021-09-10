#include "TImage.h"
//#include "TCoreSys.h"
bool TImage::CreateDIB(LONG iWidth, LONG iHeight, WORD bitCount)
{
	BITMAPINFO bmi;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = bitCount;
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;

	m_hBitmap = CreateDIBSection(g_hScreenDC, (BITMAPINFO*)&bmi,
		DIB_RGB_COLORS, (LPVOID*)&m_pRaster, NULL, 0);


	for (int j = 0; j < iHeight; ++j)
	{
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)m_pRaster)[j * iWidth];
		for (int i = 0; i < iWidth; ++i)
		{
			pbSrcRGB[0] = 0;
			pbSrcRGB[1] = 0;
			pbSrcRGB[2] = 0;
			pbSrcRGB[3] = 255;
			pbSrcRGB += 4;
		}
	}

	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	COLORREF bkColor = RGB(255, 255, 255);
	HBRUSH m_hbrRed = CreateSolidBrush(bkColor);
	SelectObject(m_hMemDC, m_hbrRed);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_bmpInfo);
	if (m_bmpInfo.bmBitsPixel == 32)
	{
		m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	}
	else
	{
		m_BlendFunction.AlphaFormat = AC_SRC_OVER;
	}
	return true;

}
bool	TImage::Init()
{
	m_BlendFunction.BlendOp = AC_SRC_OVER;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
	return true;
}

bool	TImage::Frame(){
	return true;
}
bool	TImage::Render()
{
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 0xff;
	if (m_bmpInfo.bmBitsPixel == 32)
		bf.AlphaFormat = AC_SRC_ALPHA;
	else
		bf.AlphaFormat = AC_SRC_OVER;

	if (!AlphaBlend(g_hOffScreenDC, 0, 0,	m_bmpInfo.bmWidth, m_bmpInfo.bmHeight,
					m_hMemDC, 0, 0,	m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, bf))
	{
		return false;
	}
	return true;
}
bool	TImage::Release(){
	SelectObject( m_hMemDC, m_hOldBitmap );
	DeleteObject( m_hBitmap );
	DeleteDC( m_hMemDC );
	return true;
}
bool TImage::LoadDIB(const TCHAR* pFileName)
{
	DWORD dwDataSize, dwRead;
	HANDLE hFile = CreateFile(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	ReadFile(hFile, &m_bitFileHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
	dwDataSize = m_bitFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);

	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(dwDataSize);
	ReadFile(hFile, bitInfo, dwDataSize, &dwRead, NULL);

	PVOID  pRaster = nullptr;
	m_hBitmap = CreateDIBSection(g_hScreenDC, bitInfo, DIB_RGB_COLORS, &pRaster, NULL, 0);
	ReadFile(hFile, pRaster, m_bitFileHeader.bfSize - m_bitFileHeader.bfOffBits, &dwRead, NULL);
	free(bitInfo);
	CloseHandle(hFile);
	return true;
}
bool TImage::Load(std::wstring filename)
{
	if (!LoadDIB(filename.c_str()))
	{
		return false;
	}
	/*m_hBitmap = (HBITMAP)LoadImage(
		g_hInstance,
		filename.c_str(),
		IMAGE_BITMAP,
		0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (m_hBitmap == NULL) return false;*/

	m_hMemDC = CreateCompatibleDC(g_hScreenDC);
	SelectObject(m_hMemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(BITMAP),&m_bmpInfo);
	return true;
}
bool TImage::Load(	HWND hWnd, HDC hdc, T_STR fileName )
{
	m_hMemDC = CreateCompatibleDC( hdc );
	if (!LoadDIB(fileName.c_str()))
	{
		return false;
	}
	/*m_hBitmap =(HBITMAP)LoadImage(g_hInstance,
		fileName.c_str(), 
		IMAGE_BITMAP,
		0, 0, 
		LR_DEFAULTSIZE | LR_LOADFROMFILE );*/
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC,m_hBitmap );
	GetObject(m_hBitmap, sizeof(BITMAP), &m_bmpInfo);
	return true;
}
bool    TImage::Draw(float x, float y, RECT rt, DWORD iMode)
{
	// DC -> DC
	BitBlt(g_hOffScreenDC, x, y,
		rt.right,
		rt.bottom,
		m_hMemDC,
		rt.left,
		rt.top, iMode);
	return true;
}
bool    TImage::Draw(float x, float y, RECT rt, BLENDFUNCTION bf)
{
	// DC -> DC
	float fWidth = rt.right * m_fScale;
	float fHeight = rt.bottom * m_fScale;
	float fX = x - (fWidth / 2 - rt.right / 2);
	float fY = y - (fHeight / 2 - rt.bottom / 2);

	AlphaBlend(g_hOffScreenDC, fX, fY,
		fWidth,
		fHeight,
		m_hMemDC,
		rt.left, rt.top, rt.right, rt.bottom,
		bf);
	return true;
}
bool    TImage::Draw(RECT rtDesk, RECT rtSrc, DWORD iMode)
{
	StretchBlt(g_hOffScreenDC,
		rtDesk.left, rtDesk.top,
		rtDesk.right, rtDesk.bottom,
		m_hMemDC,
		rtSrc.left, rtSrc.top, rtSrc.right, rtSrc.bottom,
		iMode);
	return true;
}
//__imp_TransparentBlt 외부 기호 : msimg32.llib
bool    TImage::DrawColorKey(float x, float y, RECT rt, COLORREF color)
{
	// DC -> DC
	TransparentBlt(
		g_hOffScreenDC, x, y, rt.right, rt.bottom,
		m_hMemDC, rt.left, rt.top, rt.right, rt.bottom,
		color);
	return true;
}
TImage::TImage(void)
{
	m_fScale = 1.0f;
	m_szName = _T("");
}


TImage::~TImage(void)
{
}
