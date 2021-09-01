#include "xObject.h"
//https://www.leunen.com/cbuilder/rotbmp.html
// CreateCompatibleBitmap함수는 16MB를 넘는 사이즈의 비트맵 생성불가
HBITMAP xObject::MakeDIBSection(HDC hdc, int width, int height)
{
	BITMAPINFO bmi;
	LPVOID pBits; 

	HBITMAP hBitmap; 
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	bmi.bmiHeader.biBitCount = 24; 
	bmi.bmiHeader.biWidth = width; 
	bmi.bmiHeader.biHeight = height; 
	bmi.bmiHeader.biPlanes = 1; 

	hBitmap = CreateDIBSection( hdc, &bmi, DIB_RGB_COLORS, (void **)&pBits, NULL, 0); 
	return hBitmap; 
} 


void		xObject::RotateBlt(float iAngle, float fAlpha)
{
	int iOffWidth = m_rtObj.right;
	int iOffHeight = m_rtObj.bottom;
	if (iOffWidth > iOffHeight)
	{
		iOffHeight = iOffWidth;
	}
	else
	{
		iOffWidth = iOffHeight;
	}

	GetRotationBimap(m_BitmapMask.m_hMemDC, m_hMaskDC, iAngle,iOffWidth,	iOffHeight);
	GetRotationBimap(m_Bitmap.m_hMemDC, m_hColorDC, iAngle,iOffWidth, iOffHeight);
	
	AlphaBlend(	m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y, iOffWidth, iOffHeight,
				m_hColorDC,	m_hMaskDC,
				0, 0, iOffWidth, iOffHeight, 
				fAlpha, SRCCOPY, RGB(0, 0, 0));

	//BitBlt(		m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,	iOffWidth, iOffHeight,	m_hMaskDC,	0, 0,	SRCAND);
	//BitBlt(		m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,	iOffWidth, iOffHeight,	m_hColorDC, 0, 0,	SRCINVERT);
	//BitBlt(		m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,	iOffWidth, iOffHeight,	m_hMaskDC,	0, 0,	SRCINVERT);
};
void xObject::GetRotationBimap(	HDC hdc, HDC srcDC,
									float fAngle,
									int iWidth,
									int iHeight)
{
	float fRadian = fAngle * 3.141592f / 180.0f;
	float fCosine = cos(fRadian);// *0.5f + 0.5f;
	float fSine = sin(fRadian);

	// 제 2의 DC 결과물이 hbmResult;	
	HBITMAP hbmResult = MakeDIBSection(m_hScreenDC, iWidth, iHeight);	
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(srcDC, hbmResult);

	COLORREF bkColor = RGB(255, 255, 255);
	HBRUSH hbrBack = CreateSolidBrush(bkColor);
	HBRUSH hbrOld = (HBRUSH)SelectObject(srcDC, hbrBack);
	PatBlt(srcDC, 0, 0, iWidth, iHeight, PATCOPY);
	DeleteObject(SelectObject(srcDC, hbrOld));

	int iOldGraphic = SetGraphicsMode(srcDC, GM_ADVANCED);
	
	XFORM xform;		
	xform.eM11 = fCosine; xform.eM12 = -fSine;
	xform.eM21 = fSine; xform.eM22 = fCosine;
	xform.eDx = iWidth / 2;
	xform.eDy = iHeight / 2;
	SetWorldTransform(srcDC, &xform);

	BitBlt(srcDC,
			-(m_rtObj.right/2.0f), 
			-(m_rtObj.bottom / 2.0f), 
			m_rtObj.right, 
			m_rtObj.bottom,
		   hdc, m_rtObj.left, m_rtObj.top, SRCCOPY);
	//SelectObject(srcDC, hbmOldDest);


	xform.eM11 = 1; xform.eM12 = 0;
	xform.eM21 = 0; xform.eM22 = 1;
	xform.eDx = 0; xform.eDy = 0;
	SetWorldTransform(srcDC, &xform);
	SetGraphicsMode(srcDC, iOldGraphic);

	DeleteObject(hbmResult);	
}
bool		xObject::Load(HDC hdc,
	HDC hOffScrenDC,
	TCHAR* pszMaskLoadFileName,
	TCHAR* pszLoadFileName)
{
	m_hScreenDC = hdc;

	if (pszMaskLoadFileName!=NULL)
	{	
		m_bMask = true;
		m_BitmapMask.Set(hdc);
		m_hOffScreenDC = hOffScrenDC;
		if (m_BitmapMask.Load(pszMaskLoadFileName) == false)
		{
			return false;
		}
		m_hMaskDC = CreateCompatibleDC(m_hScreenDC);
	}		

	m_Bitmap.Set(hdc);
	m_hOffScreenDC = hOffScrenDC;
	if (m_Bitmap.Load(pszLoadFileName)==true)
	{
		m_hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		m_hOldBrush = (HBRUSH)SelectObject(hdc, m_hBrush);
		m_hColorDC = CreateCompatibleDC(m_hScreenDC);
		return true;
	}

	
	return false;
};

bool		xObject::Init()
{
	m_Bitmap.Init();	
	return true;
};
void		xObject::SetRect( RECT rt, tPOINT pos, 
	bool bHero,
	float fSpeed )
{
	m_rtObj = rt;// { 90, 1, 40, 60 };
	m_ObjPos = pos;
	m_bHero = bHero;
	m_fSpeed = fSpeed;
	m_rtCollision.left = m_ObjPos.x;
	m_rtCollision.top = m_ObjPos.y;
	m_rtCollision.right = m_rtObj.right+ m_ObjPos.x;
	m_rtCollision.bottom = m_rtObj.bottom+ m_ObjPos.y;
}
void		xObject::Left()
{
	m_ObjPos.x -= g_fSecPerFrame * m_fSpeed;
}
void		xObject::Right()
{
	m_ObjPos.x += g_fSecPerFrame * m_fSpeed;
}
void		xObject::Up()
{
	m_ObjPos.y -= g_fSecPerFrame * m_fSpeed;
}
void		xObject::Down()
{
	m_ObjPos.y += g_fSecPerFrame * m_fSpeed;
}
bool		xObject::Render()
{
	if (m_bMask==false)
	{
		m_Bitmap.Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCCOPY);
		return true;
	}
	if (m_bMask)
	{
		m_BitmapMask.Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCAND);
		m_Bitmap.Draw(	m_hOffScreenDC, 
						m_ObjPos,
						m_rtObj, 
						m_dwReverse,
						SRCINVERT);
		m_BitmapMask.Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCINVERT);
	}

	
	return true;
};
bool		xObject::Frame()
{
	if (m_bHero == false)
	{
		if (m_ObjPos.x < g_rtClient.left)
		{
			m_ObjPos.x = 0;
			m_iDirection[0] *= -1;
		}
		if (m_ObjPos.x > g_rtClient.right)
		{
			m_ObjPos.x = g_rtClient.right;
			m_iDirection[0] *= -1;
		}

		if (m_ObjPos.y < g_rtClient.top)
		{
			m_iDirection[1] *= -1;
			m_ObjPos.y = 0;
		}
		if(	m_ObjPos.y > g_rtClient.bottom)
		{
			m_iDirection[1] *= -1;
			m_ObjPos.y = g_rtClient.bottom;
		}
		m_ObjPos.x += m_iDirection[0] * m_fSpeed * g_fSecPerFrame;
		m_ObjPos.y += m_iDirection[1] * m_fSpeed * g_fSecPerFrame;
	}
	m_rtCollision.left = m_ObjPos.x;
	m_rtCollision.top = m_ObjPos.y;
	m_rtCollision.right = m_rtObj.right + m_ObjPos.x;
	m_rtCollision.bottom = m_rtObj.bottom + m_ObjPos.y;
	return true;
};
bool		xObject::Release()
{
	DeleteDC(m_hMaskDC);
	DeleteDC(m_hColorDC);
	DeleteObject(m_hBrush);
	SelectObject(m_Bitmap.m_hScreenDC, m_hOldBrush);
	m_Bitmap.Release();
	return true;
};
bool xObject::AlphaBlend(HDC dcDest, int x, int y, int cx, int cy,
	HDC dcSrc,
	HDC dcMaskSrc, int sx, int sy, int scx, int scy, int alpha, DWORD opMode, COLORREF rgbMask)
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

	////////////////////////////////  (1)DIB 비트맵을 생성한다.  //////////////////////////////
	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// 컬러 비트맵 : pSrcBits에 직접 값을 세팅 할 수 있는 DIB 생성
	hbmSrc = CreateDIBSection(dcSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	// 배경 비트맵
	hbmDest = CreateDIBSection(dcDest, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);

	BYTE * pMaskBits;
	HBITMAP hbmMask;
	// 마스크 비트맵
	hbmMask = CreateDIBSection(dcMaskSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pMaskBits, 0, 0l);


	//////////////////////////////// (2)생성된 DIB 비트맵에 DDB 비트맵을 복사한다. //////////////////////////////
	HDC dc = CreateCompatibleDC(NULL);
	// 컬러 DIB 비트맵에 컬러 비트맵 출력
	HBITMAP dcOld = (HBITMAP)SelectObject(dc, hbmSrc);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY))
		return false;
	// 배경 DIB 비트맵에 배경 비트맵 출력
	dcOld = (HBITMAP)SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY))
		return false;
	// 마스트 DIB 비트맵에 마스크 비트맵 출력
	dcOld = (HBITMAP)SelectObject(dc, hbmMask);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcMaskSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	SelectObject(dc, dcOld);
	DeleteDC(dc);



	/////////////////////////  (3)복사된 DIB 비트맵의 픽셀을 사용하여 알파값을 부여한다. //////////////////////////////
	int red = GetRValue(rgbMask);
	int green = GetGValue(rgbMask);
	int blue = GetBValue(rgbMask);

	for (int j = 0; j < cy; ++j)
	{
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];
		LPBYTE pbMaskRGB = (LPBYTE)&((DWORD*)pMaskBits)[j * cx];

		for (int i = 0; i < cx; ++i)
		{
			if (pbMaskRGB[0] == blue &&    // 0
				pbMaskRGB[1] == green &&   // 0
				pbMaskRGB[2] == red)       // 0
			{
				// 마스크 비트맵의 흑백 픽쉘에 알파 값 만큼 배경과 컬러비트맵을 혼합한다.
				pbSrcRGB[0] = (pbDestRGB[0] * (255 - alpha) + pbSrcRGB[0] * alpha) >> 8;
				pbSrcRGB[1] = (pbDestRGB[1] * (255 - alpha) + pbSrcRGB[1] * alpha) >> 8;
				pbSrcRGB[2] = (pbDestRGB[2] * (255 - alpha) + pbSrcRGB[2] * alpha) >> 8;

			}
			else
			{
				// 마스크 비트맵의 흑백 픽쉘을 제외한 픽쉘은 배경 픽쉘로 체운다.
				pbSrcRGB[0] = pbDestRGB[0];
				pbSrcRGB[1] = pbDestRGB[1];
				pbSrcRGB[2] = pbDestRGB[2];
			}
			pbSrcRGB += 4;
			pbDestRGB += 4;
			pbMaskRGB += 4;
		}
	}
	DeleteObject(hbmDest);
	DeleteObject(hbmMask);

	///////////////////////// (4)배경에 혼합된 DIB 비트맵을 출력한다. ////////////////////////////////
	dc = CreateCompatibleDC(NULL);
	SelectObject(dc, hbmSrc);
	if (!BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, opMode))
	{
		DeleteObject(hbmSrc);
		DeleteDC(dc);
		return false;
	}
	DeleteObject(hbmSrc);
	DeleteDC(dc);
	return true;
}
void xObject::TransBitBlt(HDC hdc, int x, int y, int width, int height,
	HDC MemDC, int mx, int my, COLORREF clrMask)
{
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC hdcMem, hdcBack, hdcObject, hdcSave;

	//
	hdcSave = CreateCompatibleDC(hdc);
	bmSave = CreateCompatibleBitmap(hdc, width, height);
	bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);


	hdcBack = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem = CreateCompatibleDC(hdc);

	bmAndBack = CreateBitmap(width, height, 1, 1, NULL);
	bmAndObject = CreateBitmap(width, height, 1, 1, NULL);
	bmAndMem = CreateCompatibleBitmap(hdc, width, height);


	bmBackOld = (HBITMAP)SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
	bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);


	SetMapMode(MemDC, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, width, height, MemDC, mx, my, SRCCOPY);

	cColor = SetBkColor(MemDC, clrMask);

	BitBlt(hdcObject, 0, 0, width, height, MemDC, mx, my, SRCCOPY);

	SetBkColor(MemDC, cColor);

	BitBlt(hdcBack, 0, 0, width, height, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem, 0, 0, width, height, hdc, x, y, SRCCOPY);
	BitBlt(hdcMem, 0, 0, width, height, hdcObject, 0, 0, SRCAND);
	BitBlt(MemDC, mx, my, width, height, hdcBack, 0, 0, SRCAND);
	BitBlt(hdcMem, 0, 0, width, height, MemDC, mx, my, SRCPAINT);
	BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY);
	BitBlt(MemDC, mx, my, width, height, hdcSave, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));

	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
}
xObject::xObject()
{
	m_ObjPos.x = 100;
	m_ObjPos.y = 100;
	m_dwReverse = 0;
	m_bHero		= true;
	m_fSpeed = 70.0f;
	m_bMask = false;
	m_iDirection[0] = m_iDirection[1] =1;
}


xObject::~xObject()
{
}
