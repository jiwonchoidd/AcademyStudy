#include "TBitmap.h"
bool TBitmap::Load(const TCHAR* pszLoadFile)
{
	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
		pszLoadFile, IMAGE_BITMAP,
		0, 0, LR_DEFAULTSIZE |
		LR_LOADFROMFILE);
	if (m_hBitmap != NULL)
	{
		GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
		m_hMemDC = CreateCompatibleDC(m_hScreenDC);
		//// 생성된 메모리에 비트맵 적용
		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
		m_szName = pszLoadFile;
	}
	
	return true;
};
bool TBitmap::Load(DWORD bitmap)
{
	m_hBitmap = LoadBitmap(g_hInstance,
		MAKEINTRESOURCE(bitmap));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	// 생성된 메모리에 비트맵 적용
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	return true;
};
void		TBitmap::Set(HDC hdc)
{
	m_hScreenDC = hdc;
};
bool		TBitmap::Draw(HDC hDC, 
	tPOINT pos, 
	RECT rt,
	DWORD dwScale,
	DWORD opMode,
	BLENDFUNCTION* pAlphaBF)
{
	switch (dwScale)
	{
	case LEFT_ROTATION:
	{
		StretchBlt(
			// 목표 대상 영역
			hDC, (int)pos.x + rt.right, (int)pos.y,
			-(rt.right), rt.bottom,
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);
	}break;
	case RIGHT_ROTATION:
	{
		StretchBlt(
			// 목표 대상 영역
			hDC, (int)pos.x, (int)pos.y + rt.bottom,
			rt.right, -(rt.bottom),//+pos.y*1.33f,
								   // 원본 영역
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);
	}break;
	case 0x03:
	{
		StretchBlt(
			// 목표 대상 영역			
			hDC, (int)pos.x + rt.right,
			(int)pos.y + rt.bottom,
			-(rt.right), -(rt.bottom),
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			opMode);

		//StretchBlt(
		//	// 목표 대상 영역
		//	hDC, rt.right - rt.left,
		//	rt.bottom - rt.top,
		//	-(rt.right) + rt.left * 2,
		//	-(rt.bottom) + rt.top * 2,
		//	// 원본 영역
		//	m_hMemDC, 0, 0, m_BmpInfo.bmWidth,
		//	m_BmpInfo.bmHeight,
		//	opMode);
	}break;
	case ALPHA_DRAW:
	{
		AlphaBlend(
			// 목표 대상 영역			
			hDC, (int)pos.x, (int)pos.y,
			rt.right,
			rt.bottom, 
			m_hMemDC,
			rt.left, rt.top,
			rt.right, rt.bottom,
			*pAlphaBF);
	}break;
	default:
	{	
		BitBlt(
			// 목표 대상 영역
			hDC, (int)pos.x, (int)pos.y,
			rt.right,
			rt.bottom,
			// 원본 영역
			m_hMemDC, 
			rt.left, 
			rt.top,
			opMode);		
	}break;
	}

	return true;
};
bool    TBitmap::Draw(float x, float y, RECT rt, DWORD iMode)
{
	// DC -> DC
	BitBlt(g_hOffScreenDC, (int)x, (int)y,
		rt.right,
		rt.bottom,
		m_hMemDC,
		rt.left,
		rt.top, iMode);
	return true;
}
//__imp_TransparentBlt 외부 기호 : msing32.llib
bool    TBitmap::DrawColorKey(
	float x, float y, RECT rt, COLORREF color)
{
	// DC -> DC
	TransparentBlt(
		g_hOffScreenDC, (int)x, (int)y, rt.right, rt.bottom,
		m_hMemDC, rt.left, rt.top, rt.right, rt.bottom,
		color);
	return true;
}
bool		TBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);
	return true;
};
bool TBitmap::SaveFile(HBITMAP hBitmap, TCHAR* strSaveFile)
{
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);

	BITMAPINFOHEADER bitHeader;
	memset(&bitHeader, 0, sizeof(BITMAPINFOHEADER));
	bitHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitHeader.biWidth = bitmap.bmWidth;
	bitHeader.biHeight = bitmap.bmHeight;
	bitHeader.biPlanes = 1;
	bitHeader.biBitCount = bitmap.bmBitsPixel;
	bitHeader.biCompression = BI_RGB;
	//bitHeader.biSizeImage = bitmap.bmWidthBytes * bitmap.bmHeight;

	HDC hdc = GetDC(GetDesktopWindow());
	// biSizeImage 정보 얻기
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, NULL, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	// 비트맵 데이터 얻기
	LPBYTE lpBits = new BYTE[bitHeader.biSizeImage];
	GetDIBits(hdc, hBitmap, 0, bitmap.bmHeight, lpBits, (LPBITMAPINFO)&bitHeader, DIB_RGB_COLORS);
	ReleaseDC(GetDesktopWindow(), hdc);

	// 파일 생성 및 저장
	HANDLE hFile = CreateFile(strSaveFile, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		delete[] lpBits;
		return false;
	}
	// 파일 헤더 저장
	BITMAPFILEHEADER bmFileHeader;
	memset(&bmFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bmFileHeader.bfType = ((WORD)('M' << 8) | 'B');//MAKEWORD(
	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader.bfSize = bmFileHeader.bfOffBits + bitHeader.biSizeImage;

	DWORD dwWritten;
	//파일헤더 저장
	WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//정보헤더 저장
	WriteFile(hFile, &bitHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	//데이터 저장
	WriteFile(hFile, lpBits, bitHeader.biSizeImage, &dwWritten, NULL);
	CloseHandle(hFile);

	delete[] lpBits;
	return true;
}
TBitmap::TBitmap()
{
}


TBitmap::~TBitmap()
{
}
