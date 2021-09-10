#include "xObject.h"
bool		xObject::Load(HDC hdc,
	HDC hOffScrenDC,
	TCHAR* pszMaskLoadFileName,
	TCHAR* pszLoadFileName)
{
	if (pszMaskLoadFileName!=NULL)
	{	
		m_bMask = true;
		m_BitmapMask.Set(hdc);
		m_hOffScreenDC = hOffScrenDC;
		if (m_BitmapMask.Load(pszMaskLoadFileName) == false)
		{
			return false;
		}
	}

	m_Bitmap.Set(hdc);
	m_hOffScreenDC = hOffScrenDC;
	if (m_Bitmap.Load(pszLoadFileName)==true)
	{
		m_hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		m_hOldBrush = (HBRUSH)SelectObject(
							hdc, m_hBrush);
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
		if (m_fAlpha < 255.0f)
		{
			// ���ĺ����Ѵ�.
			AlphaBlend(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,
				m_rtObj.right,
				m_rtObj.bottom,
				// ���� ����
				m_Bitmap.m_hMemDC,
				m_BitmapMask.m_hMemDC, m_rtObj.left, m_rtObj.top, m_rtObj.right,
				m_rtObj.bottom, m_fAlpha, SRCCOPY, RGB(0, 0, 0));
		}
		else
		{
			//////// ����ũ ��Ʈ���� ����(�÷����� ���) �� ���(���� �÷� ���)���θ� �̷�� ���� �Ѵ�.
			m_BitmapMask.Draw(m_hOffScreenDC,
				m_ObjPos,
				m_rtObj,
				m_dwReverse,
				SRCAND);



			// ���� �÷� ��Ʈ�� ������ �����̸� SRCPAINT(OR)�� �ι����� �����ϴ�.
			m_Bitmap.Draw(m_hOffScreenDC,
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

	}
	if (m_bDebugRect)
	{
		// ���� white, �׵θ� red(CreatePen)
		int prevMode = SetROP2(m_hOffScreenDC, R2_NOTXORPEN);
		Rectangle(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,
			m_ObjPos.x + m_rtObj.right,
			m_ObjPos.y + m_rtObj.bottom);
		SetROP2(m_hOffScreenDC, prevMode);
	}	
	return true;
};
// DIB �� GDI�Լ��� ����� �� ����.
// �׷��� ������ DIBSection�� �����Ͽ� ���� �Ƚ��� ���� �����ؾ� �Ѵ�.
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

	////////////////////////////////  (1)DIB ��Ʈ���� �����Ѵ�.  //////////////////////////////
	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// �÷� ��Ʈ�� : pSrcBits�� ���� ���� ���� �� �� �ִ� DIB ����
	hbmSrc = CreateDIBSection(dcSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	// ��� ��Ʈ��
	hbmDest = CreateDIBSection(dcDest, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);

	BYTE * pMaskBits;
	HBITMAP hbmMask;
	// ����ũ ��Ʈ��
	hbmMask = CreateDIBSection(dcMaskSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pMaskBits, 0, 0l);


	//////////////////////////////// (2)������ DIB ��Ʈ�ʿ� DDB ��Ʈ���� �����Ѵ�. //////////////////////////////
	HDC dc = CreateCompatibleDC(NULL);
	// �÷� DIB ��Ʈ�ʿ� �÷� ��Ʈ�� ���
	HBITMAP dcOld = (HBITMAP)SelectObject(dc, hbmSrc);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY))
		return false;
	// ��� DIB ��Ʈ�ʿ� ��� ��Ʈ�� ���
	dcOld = (HBITMAP)SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY))
		return false;
	// ����Ʈ DIB ��Ʈ�ʿ� ����ũ ��Ʈ�� ���
	dcOld = (HBITMAP)SelectObject(dc, hbmMask);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcMaskSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	SelectObject(dc, dcOld);
	DeleteDC(dc);



	/////////////////////////  (3)����� DIB ��Ʈ���� �ȼ��� ����Ͽ� ���İ��� �ο��Ѵ�. //////////////////////////////
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
				// ����ũ ��Ʈ���� ��� �Ƚ��� ���� �� ��ŭ ���� �÷���Ʈ���� ȥ���Ѵ�.
				pbSrcRGB[0] = (pbDestRGB[0] * (255 - alpha) + pbSrcRGB[0] * alpha) >> 8;
				pbSrcRGB[1] = (pbDestRGB[1] * (255 - alpha) + pbSrcRGB[1] * alpha) >> 8;
				pbSrcRGB[2] = (pbDestRGB[2] * (255 - alpha) + pbSrcRGB[2] * alpha) >> 8;

			}
			else
			{
				// ����ũ ��Ʈ���� ��� �Ƚ��� ������ �Ƚ��� ��� �Ƚ��� ü���.
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

	///////////////////////// (4)��濡 ȥ�յ� DIB ��Ʈ���� ����Ѵ�. ////////////////////////////////
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
void xObject::TransBitBlt(	HDC hdc, int x, int y, int width, int height,
							HDC MemDC, int mx, int my, COLORREF clrMask)
{
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC hdcMem, hdcBack, hdcObject, hdcSave;

	//
	hdcSave	  = CreateCompatibleDC(hdc);
	bmSave	  = CreateCompatibleBitmap(hdc, width, height);
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
bool		xObject::Frame()
{
	if (m_bHero) return true;
	if (m_ObjPos.x < g_rtClient.left)
	{
		m_iDirection[0] *= -1;
		m_ObjPos.x = g_rtClient.left;
	}
	if (m_ObjPos.x > g_rtClient.right)
	{
		m_iDirection[0] *= -1;
		m_ObjPos.x = g_rtClient.right;
	}
	if (m_ObjPos.y < g_rtClient.top )
	{
		m_iDirection[1] *= -1;
		m_ObjPos.y = g_rtClient.top;
	}
	if (m_ObjPos.y > g_rtClient.bottom)
	{
		m_iDirection[1] *= -1;
		m_ObjPos.y = g_rtClient.bottom;
	}
	m_ObjPos.x += m_iDirection[0] * m_fSpeed * g_fSecPerFrame;
	m_ObjPos.y += m_iDirection[1] * m_fSpeed * g_fSecPerFrame;
	return true;
};
bool		xObject::Release()
{
	DeleteObject(m_hBrush);
	SelectObject(m_Bitmap.m_hScreenDC, m_hOldBrush);
	m_Bitmap.Release();
	return true;
};

xObject::xObject()
{
	m_ObjPos.x = 100;
	m_ObjPos.y = 100;
	m_dwReverse = 0;
	m_bHero		= true;
	m_fSpeed = 70.0f;
	m_bMask = false;
	m_fAlpha = 255.0f;
	m_bDebugRect = true;
	m_iDirection[0] = m_iDirection[1] =1;
}


xObject::~xObject()
{
}
