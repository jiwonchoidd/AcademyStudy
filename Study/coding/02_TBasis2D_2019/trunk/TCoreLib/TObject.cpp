#include "TObject.h"
#include "TStd.h"
#include "TImageMgr.h"
void     TObject::SetPos(POINT pos)
{
	m_vPos.x = pos.x;
	m_vPos.y = pos.y;
	SetUpdateData();
}
void     TObject::SetPos(TPoint pos)
{
	m_vPos = pos;
	SetUpdateData();
}
void	TObject::SetPos(float iX, float iY)
{
	m_vPos.x = iX;
	m_vPos.y = iY;
	SetUpdateData();
}
void TObject::SetUpdateData()
{
	int xStep = m_SrcRect.right / 2;
	int yStep = m_SrcRect.bottom / 2;

	int left = m_vPos.x - xStep;
	int right = m_vPos.x + xStep;
	int top = m_vPos.y - yStep;
	int bottom = m_vPos.y + yStep;

	m_DeskRect.left = left;
	m_DeskRect.top = top;
	m_DeskRect.right = m_SrcRect.right;
	m_DeskRect.bottom = m_SrcRect.bottom;

	m_rtCollision.left = m_DeskRect.left;
	m_rtCollision.top = m_DeskRect.top;
	m_rtCollision.right = right;
	m_rtCollision.bottom = bottom;
}
HBITMAP TObject::MakeDIBSection(HDC hdc, int width, int height, int iBitCount)
{
	BITMAPINFO bmi;
	LPVOID pBits;

	HBITMAP hBitmap;
	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = iBitCount;
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;

	hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void **)&pBits, NULL, 0);
	return hBitmap;
}

void	TObject::RotateBlt(float iAngle, float fAlpha)
{
	int iOffWidth = m_SrcRect.right;
	int iOffHeight = m_SrcRect.bottom;
	if (iOffWidth > iOffHeight)
	{
		iOffHeight = iOffWidth;
	}
	else
	{
		iOffWidth = iOffHeight;
	}

	GetRotationBimap(m_pImageMask->m_hMemDC, m_hMaskDC, iAngle, iOffWidth, iOffHeight);
	GetRotationBimap(m_pImage->m_hMemDC, m_hColorDC, iAngle, iOffWidth, iOffHeight);

	AlphaBlend(g_hOffScreenDC, m_vPos.x, m_vPos.y, iOffWidth, iOffHeight,
		m_hColorDC, m_hMaskDC,
		0, 0, iOffWidth, iOffHeight,
		fAlpha, SRCCOPY, RGB(0, 0, 0));

	//BitBlt(		g_hOffScreenDC, m_vPos.x, m_vPos.y,	iOffWidth, iOffHeight,	m_hMaskDC,	0, 0,	SRCAND);
	//BitBlt(		g_hOffScreenDC, m_vPos.x, m_vPos.y,	iOffWidth, iOffHeight,	m_hColorDC, 0, 0,	SRCINVERT);
	//BitBlt(		g_hOffScreenDC, m_vPos.x, m_vPos.y,	iOffWidth, iOffHeight,	m_hMaskDC,	0, 0,	SRCINVERT);
};
void TObject::GetRotationBimap(HDC hdc, HDC srcDC,
	float fAngle,
	int iWidth,
	int iHeight)
{
	float fRadian = fAngle * 3.141592f / 180.0f;
	float fCosine = cos(fRadian);// *0.5f + 0.5f;
	float fSine = sin(fRadian);

	// 제 2의 DC 결과물이 hbmResult;	
	HBITMAP hbmResult = MakeDIBSection(g_hScreenDC, iWidth, iHeight,24);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(srcDC, hbmResult);
	// m_hRotationDC 클리어
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
		-(m_SrcRect.right / 2.0f),
		-(m_SrcRect.bottom / 2.0f),
		m_SrcRect.right,
		m_SrcRect.bottom,
		hdc, m_SrcRect.left, m_SrcRect.top, SRCCOPY);

	xform.eM11 = 1; xform.eM12 = 0;
	xform.eM21 = 0; xform.eM22 = 1;
	xform.eDx = 0; xform.eDy = 0;
	SetWorldTransform(srcDC, &xform);
	SetGraphicsMode(srcDC, iOldGraphic);

	DeleteObject(hbmResult);
}

bool TObject::Init()
{
	m_hDrawDC = g_hOffScreenDC;
	return true;
}
bool  TObject::Set(TObjectInfo info, int id, int mask)
{
	m_info = info;
	m_info.bLoop = false;
	if (info.fLifeTime == 0.0f)
	{
		m_info.bLoop = true;
	}
	if (m_info.iBitmapID == -1)
	{
		m_pImage = I_ImageMgr.GetPtr(id);
	}
	else
	{
		m_pImage = I_ImageMgr.GetPtr(m_info.iBitmapID);
	}
	if (m_info.iMaskID == -1)
	{
		m_pImageMask = I_ImageMgr.GetPtr(mask);
	}
	else
	{
		m_pImageMask = I_ImageMgr.GetPtr(m_info.iMaskID);
	}
	m_fDeltaTime = m_info.fTime / m_info.iNumFrame;
	m_vPos = m_info.InitPos;
	return true;
}
bool TObject::SetSourceRect(float iX, float iY, RECT rt, float fSpeed)
{	
	m_SrcRect = rt;
	m_fSpeed = fSpeed;
	SetPos(iX, iY);
	return true;
}
void	 TObject::Move(TPoint pos)
{
	TBASIS_VECTOR::Vector2 s(m_vPos);
	TBASIS_VECTOR::Vector2 e(pos);
	TBASIS_VECTOR::Vector2 d(e - s);
	if (fabsf(d.Length() - 0.0f) >= 0.01f)
	{
		s = s + d.Normal() * m_fSpeed * g_fSecPerFrame;
	}
	SetPos(s.x, s.y);
}
bool TObject::Load(	const TCHAR* strLoadFile,
					const TCHAR* strMaskLoadFile)
{
	Init();
	if( strLoadFile != NULL )
	{
		m_iIndex = I_ImageMgr.Load(g_hWnd,	m_hDrawDC, strLoadFile);
		if( m_iIndex <= 0  ) return false;
		m_pImage = I_ImageMgr.GetPtr(m_iIndex);
		m_hColorDC = CreateCompatibleDC(g_hScreenDC);
	}
	if( strMaskLoadFile != NULL )
	{
		m_iIndexMask = I_ImageMgr.Load(g_hWnd,	m_hDrawDC, strMaskLoadFile);
		if( m_iIndexMask <= 0 ) return false;
		m_pImageMask = I_ImageMgr.GetPtr(m_iIndexMask);
		m_hMaskDC = CreateCompatibleDC(g_hScreenDC);
	}
	return true;	
}
bool TObject::Frame()
{
	this->m_AlphaBlend.BlendOp = AC_SRC_OVER;
    this->m_AlphaBlend.BlendFlags = 0;
	this->m_AlphaBlend.SourceConstantAlpha = m_iAlpha;
    this->m_AlphaBlend.AlphaFormat = AC_SRC_OVER;

	if( m_bRotation )
	{
		m_fAngle += (timeGetTime() % 100 ) * g_fSecPerFrame;
	}
	if (m_info.bLoop == false)
	{
		m_info.fLifeTime -= g_fSecPerFrame;
		if (m_info.fLifeTime <= 0) m_bDead = true;
	}
	if (m_bDead == true) return true;
	if (m_info.iNumFrame <= 1) return true;

	m_fTmpTime += g_fSecPerFrame;
	if (m_fTmpTime > m_fDeltaTime)
	{
		m_iCurrentFrame++;
		if (m_iCurrentFrame >= m_info.iNumFrame)
		{
			m_iCurrentFrame = 0;
		}
		m_fTmpTime = 0.0f;
	}
	return true;
}
bool TObject::Render()
{
	if (m_bDead == true) return true;

	if( m_bRotation )
	{
		int iWidth = m_DeskRect.right;
		int iHeight = m_DeskRect.bottom;
		if( iWidth > iHeight ) iHeight = iWidth;
		else                   iWidth = iHeight;

		RotateBlt(m_fAngle, m_fAlpha);		
		return true;
	}
	
	if( m_pImageMask == NULL || m_pImageMask->m_hBitmap == NULL )
	{
		if (m_info.rtList.size() > 0)
		{
			m_pImage->Draw(m_vPos.x, m_vPos.y,	m_info.rtList[m_iCurrentFrame], MERGEPAINT);
		}
		else
		{
			Draw(m_hDrawDC,
				m_DeskRect.left, m_DeskRect.top,
				m_DeskRect.right, m_DeskRect.bottom,
				m_pImage->m_hMemDC, m_SrcRect.left, m_SrcRect.top,
				SRCCOPY);
		}
	}
	else
	{
		if (m_info.rtList.size() > 0)
		{
			m_pImageMask->Draw(m_vPos.x, m_vPos.y, m_info.rtList[m_iCurrentFrame], SRCAND);
			m_pImage->Draw(m_vPos.x, m_vPos.y, m_info.rtList[m_iCurrentFrame], SRCINVERT);
			m_pImageMask->Draw(m_vPos.x, m_vPos.y, m_info.rtList[m_iCurrentFrame], SRCINVERT);
		}

		if (m_fAlpha < 255.0f)
		{
			AlphaBlend(m_hDrawDC, m_DeskRect.left, m_DeskRect.top,
				m_DeskRect.right, m_DeskRect.bottom,
				// 원본 영역
				m_pImage->m_hMemDC,
				m_pImageMask->m_hMemDC, 
				m_SrcRect.left, m_SrcRect.top, m_DeskRect.right, m_DeskRect.bottom,
				m_fAlpha, SRCCOPY, RGB(0, 0, 0));
		}
		else
		{	
			Draw(m_hDrawDC,
				m_DeskRect.left, m_DeskRect.top,
				m_DeskRect.right, m_DeskRect.bottom,
				m_pImageMask->m_hMemDC, m_SrcRect.left, m_SrcRect.top,
				SRCAND);
			Draw(m_hDrawDC, m_DeskRect.left, m_DeskRect.top,
				m_DeskRect.right, m_DeskRect.bottom,
				m_pImage->m_hMemDC, m_SrcRect.left, m_SrcRect.top,
				SRCINVERT);
			Draw(m_hDrawDC, m_DeskRect.left, m_DeskRect.top,
				m_DeskRect.right, m_DeskRect.bottom,
				m_pImageMask->m_hMemDC, m_SrcRect.left, m_SrcRect.top,
				SRCINVERT);					
		}		
	}
	//R2_XORPEN    //XOR 연산으로 겹치는 부분만 반전처리
	//R2_NOTXORPEN //XOR 결과의 반대값   
	if (m_bDebugRect)
	{
		int save = SetROP2(m_hDrawDC, R2_NOTXORPEN);
		Rectangle(m_hDrawDC, m_DeskRect.left, m_DeskRect.top,
			m_DeskRect.left + m_DeskRect.right,
			m_DeskRect.top + m_DeskRect.bottom);
		SetROP2(m_hDrawDC, save);
	}
	return true;
}
bool TObject::Release()
{
	DeleteDC(m_hMaskDC);
	DeleteDC(m_hColorDC);
	ReleaseDC( g_hWnd, m_hDrawDC );
	return true;
}
void  TObject::Draw(DWORD dwMode)
{
	if (m_pImage == nullptr) return;
	m_pImage->Draw(m_vPos.x, m_vPos.y,	m_info.rtList[m_iCurrentFrame], dwMode);
}
void  TObject::Draw(int x, int y, DWORD dwMode)
{
	if (m_pImage == nullptr) return;
	m_pImage->Draw(x, y,	m_info.rtList[m_iCurrentFrame], dwMode);
}
void TObject::Draw(	HDC hdc, int iX, int iY, int iWidth, int iHeight,
					HDC srchdc, int iStartX, int iStartY,						
					DWORD rop)
{
	int save = SetStretchBltMode(hdc, HALFTONE);
	StretchBlt(	hdc, iX, iY, iWidth, iHeight,
				srchdc, iStartX, iStartY,iWidth, iHeight, rop);
	SetStretchBltMode(hdc, save);
}
void TObject::Draw(	HDC hdc,int iX, int iY,int iWidth, int iHeight,
					HDC srchdc,int iStartX, int iStartY, int iSrcWidth, int iSrcHeight,
					DWORD rop)
{
	SetStretchBltMode(hdc, HALFTONE );
	StretchBlt(	hdc, iX, iY, iWidth, iHeight,
				srchdc, iStartX, iStartY, iSrcWidth, iSrcHeight, rop);
}
void TObject::DrawColorKey(HDC hdc, int iX, int iY, int iWidth, int iHeight,
	HDC srchdc, int iStartX, int iStartY, int iSrcWidth, int iSrcHeight,
	DWORD rop, DWORD dwColorKey)
{
	SetStretchBltMode(hdc, HALFTONE);
	TransparentBlt(
		hdc, iX, iY, iWidth, iHeight,
		srchdc, iStartX, iStartY, iSrcWidth, iSrcHeight,
		dwColorKey);
}
void TObject::DrawAlphaBlend(HDC hdc, int iX, int iY, int iWidth, int iHeight,
	HDC srchdc, int iStartX, int iStartY, int iSrcWidth, int iSrcHeight , float fAlpha)
{
	//소스 사각형과 대상 사각형의 크기가 같지 않으면 소스 비트 맵이 대상 사각형과 일치하도록 늘어난다.
	//SetStretchBltMode(hdc, HALFTONE); //사용하여도 COLORONCOLOR로 변경된다.
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = fAlpha;
	if (m_pImage->m_bmpInfo.bmBitsPixel == 32)
		bf.AlphaFormat = AC_SRC_ALPHA;
	else
		bf.AlphaFormat = AC_SRC_OVER;

	::AlphaBlend(	hdc, iX, iY, iWidth, iHeight,
					srchdc, iStartX, iStartY, iSrcWidth, iSrcHeight, bf);
}
void TObject::DrawBitmap(	HDC hdc, 
							int iX, int iY, int iWidth, int iHeight,
							int iStartX, int iStartY,
							HBITMAP bitmap,	DWORD rop)
{
	// 화면 DC와 호환가능한 DC를 사용
	// 메모리 DC
	HDC MemDC = CreateCompatibleDC(hdc);
		// 저장
		HBITMAP OldBitmap = (HBITMAP)SelectObject( MemDC, bitmap );
			
			BitBlt( hdc, iX, iY, iWidth, iHeight,// B로
					MemDC, iStartX, iStartY, rop ); //A에서
		// 복원
		SelectObject( MemDC, OldBitmap);
	// 소멸
	DeleteDC( MemDC );	
}
bool TObject::AlphaBlend(HDC dcDest, int x, int y, int cx, int cy,
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

	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// Create DIB section in shared memory
	hbmSrc = CreateDIBSection(dcSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	// Create DIB section in shared memory
	hbmDest = CreateDIBSection(dcDest, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);


	BYTE * pMaskBits;
	HBITMAP hbmMask;
	// Create DIB section in shared memory
	hbmMask = CreateDIBSection(dcMaskSrc, (BITMAPINFO *)&BMI, DIB_RGB_COLORS, (void **)&pMaskBits, 0, 0l);

	HDC dc = CreateCompatibleDC(NULL);

	HBITMAP dcOld = (HBITMAP)SelectObject(dc, hbmSrc);

	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	dcOld = (HBITMAP)SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY))
		return false;

	dcOld = (HBITMAP)SelectObject(dc, hbmMask);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcMaskSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	SelectObject(dc, dcOld);
	DeleteDC(dc);


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
			if (pbMaskRGB[0] == blue &&
				pbMaskRGB[1] == green &&
				pbMaskRGB[2] == red)
			{
				pbSrcRGB[0] = (pbDestRGB[0] * (255 - alpha) + pbSrcRGB[0] * alpha) >> 8;
				pbSrcRGB[1] = (pbDestRGB[1] * (255 - alpha) + pbSrcRGB[1] * alpha) >> 8;
				pbSrcRGB[2] = (pbDestRGB[2] * (255 - alpha) + pbSrcRGB[2] * alpha) >> 8;

			}
			else
			{
				pbSrcRGB[0] = pbDestRGB[0];
				pbSrcRGB[1] = pbDestRGB[1];
				pbSrcRGB[2] = pbDestRGB[2];
			}
			pbSrcRGB += 4;
			pbDestRGB += 4;
			pbMaskRGB += 4;
		}
	}

	dc = CreateCompatibleDC(NULL);

	SelectObject(dc, hbmSrc);
	if (!BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, opMode))
		return false;
	DeleteDC(dc);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);
	DeleteObject(hbmMask);
	return true;
}

void TObject::TransBitBlt(	HDC hdc, int x, int y, int width, int height,
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
TObject::TObject(void)
{
	m_pImageMask = NULL;
	m_pImage	 = NULL;
	m_iAlpha	 = 255;
	m_vPos.x = 0.0f;
	m_vPos.y = 0.0f;
	m_fSpeed = 300.0f;
	m_hDrawDC		= NULL;
	m_hOldBitmap= NULL;
	SetRect( &m_SrcRect, 0, 0, 0, 0 );
	SetRect( &m_DeskRect, 0, 0, 0, 0 );
	m_bCollide	= false;
	m_bRotation = false;
	m_fAngle	= 0.0f;
	m_bDebugRect = true;
	m_bDead = false;
	m_fAlpha = 255.0f;
}


TObject::~TObject(void)
{
}
