#include "xObject.h"
//BLACKNESS : �̹����� ������ ���� �ƴ� ���������� ä��ϴ�.
//DSTINVERT : ������ �̹����ʹ� ������� ȭ���� ������ ������ŵ�ϴ�.
//MERGECOPY : �̹����� ����� ���� ���õ� �귯�ø� AND �����ڸ� ����Ͽ� �����մϴ�.
//MERGEPAINT : ������ �̹����� ȭ���� ���� OR �����ڸ� ����Ͽ� �����մϴ�.
//NOMIRRORBITMAP : Prevents the bitmap from being mirrored.
//NOTSRCCOPY : �̹����� ������ ������ŵ�ϴ�.
//NOTSRCERASE : ȭ��� �̹����� OR �����ڸ� ����Ͽ� ��ģ ���� ������ ������ŵ�ϴ�.
//PATCOPY : ���� ���õ� �귯�ø� ����մϴ�.
//PATINVERT : ���� ���õ� �귯�� ���� ȭ���� XOR �����ڸ� ����Ͽ� �����մϴ�.
//PATPAINT : ���� ���õ� �귯�� ���� ������ ���� �̹����� OR �����ڸ� ����Ͽ� �����մϴ�.
//�״��� OR �����ڸ� ����Ͽ� ȭ��� �����մϴ�.
//SRCAND : ȭ��� �̹����� AND �����ڸ� ����Ͽ� �����մϴ�.
//SRCCOPY : ���� �̹����� ����մϴ�.
//SRCERASE : ������ ȭ��� �̹����� AND �����ڸ� ����Ͽ� �����մϴ�.
//SRCINVERT : ȭ��� �̹����� XOR �����ڸ� ����Ͽ� �����մϴ�.
//SRCPAINT : ȭ��� �̹����� OR �����ڸ� ����Ͽ� �����մϴ�.
//WHITENESS : �̹����� ������ ���� �ƴ� �Ͼ������ ä��ϴ�.
HBITMAP xObject::Create_MaskBitmap(HBITMAP initBitmap, COLORREF in_transColor)
{
	HDC     memDC_org, memDC_mask, memDC_ret;
	HBITMAP bitmap_mask;
	HBITMAP bitmap_maskOld, bmSaveOld, bitmap_retOld;
	BITMAP  bitmap_info;

	GetObject(initBitmap, sizeof(BITMAP), &bitmap_info);
	bitmap_mask = CreateBitmap(bitmap_info.bmWidth, bitmap_info.bmHeight, 1, 1, NULL);
	// ������ ��Ʈ�� ����
	HBITMAP hbmResult = CreateCompatibleBitmap(m_hScreenDC, bitmap_info.bmWidth, bitmap_info.bmHeight);

	memDC_org = CreateCompatibleDC(NULL);
	memDC_mask = CreateCompatibleDC(NULL);
	memDC_ret = CreateCompatibleDC(NULL);
	bmSaveOld = (HBITMAP)SelectObject(memDC_org, initBitmap);
	bitmap_maskOld = (HBITMAP)SelectObject(memDC_mask, bitmap_mask);
	bitmap_retOld = (HBITMAP)SelectObject(memDC_ret, hbmResult);
	
	// in_transColor�� ����� �ǰ� �������� �������� �ȴ�.
	COLORREF crOldBack = SetBkColor(memDC_org, in_transColor);
	BitBlt(memDC_mask, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_org, 0, 0, SRCCOPY);
	BitBlt(memDC_ret, 0, 0, bitmap_info.bmWidth, bitmap_info.bmHeight, memDC_mask, 0, 0, SRCINVERT);
	SetBkColor(memDC_org, crOldBack);

	SelectObject(memDC_org, bmSaveOld);
	SelectObject(memDC_mask, bitmap_maskOld);
	SelectObject(memDC_ret, bitmap_retOld);
	DeleteDC(memDC_org);
	DeleteDC(memDC_mask);
	DeleteDC(memDC_ret);
	DeleteObject(bitmap_mask);
	return hbmResult;
}
// ����ũ ��Ʈ�� ����� ��ο� �ϱ�.
// ��, ���� �̹����� ����ũ �÷����� �׻� ������� ó���ȴ�.
// �׷��� ������ ������ ����ũ �÷��� �ش��ϴ� �Ƚ��� ����̾�� �Ѵ�.
// ����� �ƴ� ���� ������ ����ũ �÷����� �ش��ϴ� �Ƚ��� ������� ó�� �� 
// ���Ŀ� 3���� bitBlt�� ��ο��Ѵ�.
// �׷��� ������ ����ũ ��Ʈ�ʰ� OR �������� = ��Ʈ�� �����Ͽ� ��ο� �ϸ� �ȴ�.
bool		xObject::RenderMask(COLORREF clrMask)
{
	HBITMAP old = (HBITMAP)SelectObject(m_pBitmap->m_hMemDC, m_pBitmap->m_hOldBitmap);
	////////////////////��� -1-////////////////////////
	HBITMAP newBitmap = Create_MaskBitmap(m_pBitmap->m_hBitmap, clrMask);
	HDC m_hMaskDC = CreateCompatibleDC(m_hScreenDC);
	HBITMAP hOldMask = (HBITMAP)SelectObject(m_hMaskDC, newBitmap);

	// ����ũ �̹��� ��ο�( ��� )
	BitBlt(m_hOffScreenDC,
		m_ObjPos.x, m_ObjPos.y,	m_rtObj.right, m_rtObj.bottom,
		m_hMaskDC, m_rtObj.left, m_rtObj.top, SRCAND);

	// ���� ����
	SelectObject(m_pBitmap->m_hMemDC, old);
	BitBlt(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,	m_rtObj.right, m_rtObj.bottom,
		m_pBitmap->m_hMemDC, m_rtObj.left, m_rtObj.top, SRCINVERT);

	BitBlt(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,	m_rtObj.right, m_rtObj.bottom,
		m_hMaskDC, m_rtObj.left, m_rtObj.top, SRCINVERT);

	SelectObject(m_hMaskDC, hOldMask);
	DeleteDC(m_hMaskDC);
	DeleteObject(newBitmap);
	
	//////////////////��� -2-////////////////////////
	//DrawMask(m_hOffScreenDC, 
	//	m_ObjPos.x, 
	//	m_ObjPos.y, 
	//	m_pBitmap->m_hBitmap, clrMask );

	//SelectObject(m_pBitmap->m_hMemDC,old);
	return true;
}
void xObject::DrawMask(HDC hdc, int x, int y, 
	 HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP  bm;
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize;
	// hdc = m_hOffScreenDC;
	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	// ����̽� ��ǥ�� �� ��ǥ�� ��ȯ
	// ����̽� ��ǥ :������� ���� ��ǥ
	// �� ��ǥ : ��ǻ�Ͱ� ����ϴ� �ػ� ��ǥ
	// �ػ󵵿� ������� ������� ���� ������ �׷����� ����ϱ� ���ؼ� ����.
	//DPtoLP(hdcTemp, &ptSize, 1);

	hdcBack = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem = CreateCompatibleDC(hdc);
	hdcSave = CreateCompatibleDC(hdc);
	// ����̹���
	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	//�÷��̹���
	bmAndMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

	bmBackOld = (HBITMAP)SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
	bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);
	bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hdc));
	// �۾� �� ���� �ӽ� ����
	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	
	
	// ���� �÷� ����
	cColor = SetBkColor(hdcTemp, clrMask);
	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	//����
	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, 
			hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, 
			hdc, x, y, SRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, 
			hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, 
			hdcBack, 0, 0, SRCAND);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, 
			hdcTemp, 0, 0, SRCPAINT);
	BitBlt(hdc, x, y, ptSize.x, ptSize.y, 
		hdcMem, 0, 0, SRCCOPY);
	
	// �۾��� ��Ȳ���� ����
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, 
			hdcSave, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));

	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}
void		xObject::RotateBlt(float iAngle)
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

	GetRotationBimap(m_pBitmapMask->m_hMemDC, m_hhMaskRotateBitmap, iAngle, iOffWidth, iOffHeight);
	GetRotationBimap(m_pBitmap->m_hMemDC, m_hColorRotateBitmap, iAngle, iOffWidth, iOffHeight);

	HBITMAP hOldMask = (HBITMAP)SelectObject(m_hMaskDC, m_hhMaskRotateBitmap);
	HBITMAP hOldColor = (HBITMAP)SelectObject(m_hColorDC, m_hColorRotateBitmap);

	BitBlt(m_hOffScreenDC,
		m_ObjPos.x, m_ObjPos.y,
		iOffWidth, iOffHeight,
		m_hMaskDC, 0, 0, SRCAND);
	BitBlt(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,
		iOffWidth, iOffHeight,
		m_hColorDC, 0, 0, SRCINVERT);
	BitBlt(m_hOffScreenDC, m_ObjPos.x, m_ObjPos.y,
		iOffWidth, iOffHeight,		// ���� ����
		m_hMaskDC, 0, 0, SRCINVERT);

	SelectObject(m_hMaskDC, hOldMask);
	SelectObject(m_hColorDC, hOldColor);

};
HBITMAP xObject::GetRotationBimap(HDC hdc, HBITMAP hbmResult,
	float fAngle,
	int iWidth,
	int iHeight)
{
	float fRadian = fAngle * 3.141592f / 180.0f;
	float fCosine = cos(fRadian);// *0.5f + 0.5f;
	float fSine = sin(fRadian);

	// �� 2�� DC ������� hbmResult;	
	//HBITMAP hbmResult = MakeDIBSection(m_hScreenDC, iWidth, iHeight);	
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(m_hRotationDC, hbmResult);
	// m_hRotationDC Ŭ����
	COLORREF bkColor = RGB(255, 255, 255);
	HBRUSH hbrBack = CreateSolidBrush(bkColor);
	HBRUSH hbrOld = (HBRUSH)SelectObject(m_hRotationDC, hbrBack);
	PatBlt(m_hRotationDC, 0, 0, iWidth, iHeight, PATCOPY);
	DeleteObject(SelectObject(m_hRotationDC, hbrOld));

	int iOldGraphic = SetGraphicsMode(m_hRotationDC, GM_ADVANCED);

	XFORM xform;
	xform.eM11 = fCosine; xform.eM12 = -fSine;
	xform.eM21 = fSine; xform.eM22 = fCosine;
	xform.eDx = iWidth / 2;
	xform.eDy = iHeight / 2;
	SetWorldTransform(m_hRotationDC, &xform);

	BitBlt(m_hRotationDC,
		-(m_rtObj.right / 2.0f),
		-(m_rtObj.bottom / 2.0f),
		m_rtObj.right,
		m_rtObj.bottom,
		hdc, m_rtObj.left, m_rtObj.top, SRCCOPY);
	SelectObject(m_hRotationDC, hbmOldDest);


	xform.eM11 = 1; xform.eM12 = 0;
	xform.eM21 = 0; xform.eM22 = 1;
	xform.eDx = 0; xform.eDy = 0;
	SetWorldTransform(m_hRotationDC, &xform);
	SetGraphicsMode(m_hRotationDC, iOldGraphic);
	return hbmResult;
}
bool		xObject::Load(HDC hdc,
	HDC hOffScrenDC,
	TCHAR* pszMaskLoadFileName,
	TCHAR* pszLoadFileName)
{
	m_hScreenDC = hdc;
	m_hRotationDC = CreateCompatibleDC(m_hScreenDC);
	if (pszMaskLoadFileName!=NULL)
	{	
		m_bMask = true;		
		m_hOffScreenDC = hOffScrenDC;
		m_iBitmapMask = I_BMgr.Add(pszMaskLoadFileName);
		m_pBitmapMask = I_BMgr.GetPtr(m_iBitmapMask);
		m_pBitmapMask->Set(hdc);
		if (m_iBitmapMask < 0)
		{
			return false;
		}
		m_hMaskDC = CreateCompatibleDC(m_hScreenDC);
	}
	
	m_hOffScreenDC = hOffScrenDC;
	m_iBitmap = I_BMgr.Add(pszLoadFileName);
	m_pBitmap = I_BMgr.GetPtr(m_iBitmap);
	m_pBitmap->Set(hdc);
	if (m_iBitmap >= 0)
	{	
		m_hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		m_hOldBrush = (HBRUSH)SelectObject(
							hdc, m_hBrush);
		m_hColorDC = CreateCompatibleDC(m_hScreenDC);
		return true;
	}
	return false;
};


bool		xObject::Init()
{
	//typedef struct _BLENDFUNCTION
	//{
	//	BYTE   BlendOp;
	//	BYTE   BlendFlags;
	//	BYTE   SourceConstantAlpha;
	//	BYTE   AlphaFormat;
	//}BLENDFUNCTION, *PBLENDFUNCTION;
	m_AlphaBlendFuction.BlendOp = AC_SRC_OVER;
	m_AlphaBlendFuction.BlendFlags = 0;
	m_AlphaBlendFuction.SourceConstantAlpha = m_fAlpha;
	m_AlphaBlendFuction.AlphaFormat = AC_SRC_OVER;
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

	int iDistance = sqrt(m_rtObj.right*m_rtObj.right + m_rtObj.bottom*m_rtObj.bottom);
	m_hColorRotateBitmap = CreateCompatibleBitmap(m_hScreenDC, iDistance, iDistance);
	m_hhMaskRotateBitmap = CreateCompatibleBitmap(m_hScreenDC, iDistance, iDistance);
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
bool		xObject::RenderAlpha()
{
	if (m_bMask == false)
	{
		m_AlphaBlendFuction.SourceConstantAlpha = m_fAlpha;
		m_pBitmap->Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			ALPHA_DRAW,
			SRCCOPY,
			&m_AlphaBlendFuction);
		return true;
	}
	return true;
}
bool		xObject::RenderScrool()
{
	static float fStep = 0.0f;
	fStep += g_fSecPerFrame*100.0f;
	if (fStep > m_rtObj.bottom) fStep = 0;
	int scrool = m_rtObj.bottom - fStep;

	BitBlt(
		// ��ǥ ��� ����
		m_hOffScreenDC,	0,	fStep,	m_rtObj.right,	m_rtObj.bottom - fStep,
		// ���� ����
		m_pBitmap->m_hMemDC,	0,	0,
		SRCCOPY);

	BitBlt(
		// ��ǥ ��� ����
		m_hOffScreenDC, 
		0, // ���� ��ġ x
		0, // ���� ��ġ y
		m_rtObj.right, //  ���� ��
		fStep, // ���� ��
		// ���� ����
		m_pBitmap->m_hMemDC,
		0, // �̹��� ���� ��ġ x
		scrool,      // �̹��� ���� ��ġ y
		SRCCOPY);

	
	return true;
}
bool		xObject::Render()
{
	if (m_bMask==false)
	{
		m_pBitmap->Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCCOPY);
		return true;
	}
	if (m_bMask)
	{
		m_pBitmapMask->Draw(m_hOffScreenDC,
			m_ObjPos,
			m_rtObj,
			m_dwReverse,
			SRCAND);
		m_pBitmap->Draw(	m_hOffScreenDC, 
						m_ObjPos,
						m_rtObj, 
						m_dwReverse,
						SRCINVERT);
		m_pBitmapMask->Draw(m_hOffScreenDC,
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
		if (m_ObjPos.x < g_rtClient.left + 10 ||
			m_ObjPos.x > g_rtClient.right - 60)
		{
			m_iDirection[0] *= -1;
		}
		if (m_ObjPos.y < g_rtClient.top + 10 ||
			m_ObjPos.y > g_rtClient.bottom - 80)
		{
			m_iDirection[1] *= -1;
		}
		float fTime = g_fSecPerFrame;	
		m_ObjPos.x += m_iDirection[0] * m_fSpeed * fTime;
		m_ObjPos.y += m_iDirection[1] * m_fSpeed * fTime;
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
	DeleteDC(m_hRotationDC);
	DeleteObject(m_hhMaskRotateBitmap);
	DeleteObject(m_hColorRotateBitmap);

	DeleteObject(m_hBrush);
	SelectObject(m_pBitmap->m_hScreenDC, m_hOldBrush);
	return true;
};

xObject::xObject()
{
	m_fAlpha = 0;
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
