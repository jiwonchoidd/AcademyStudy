#include "Sample.h"
#include <math.h>

#define RadianToDegree(fAngle)  ( fAngle * (3.14192f / 180.0f))   
#define DegreeToRadian(fAngle)  ( fAngle * ( 180.0f / 3.14192f) )   


bool Sample::Frame()
{
	m_fAngle += g_fSecPerFrame * 50.0f;
	return true;
}
bool Sample::Init()
{	
	m_hMaskMemDC = CreateCompatibleDC(g_hScreenDC);
	m_hColorMemDC = CreateCompatibleDC(g_hScreenDC);
	m_RotationBitmapDC = CreateCompatibleDC(g_hScreenDC);

	COLORREF bkColor = RGB(255, 255, 255);
	m_back = CreateSolidBrush(bkColor);
		
	m_rt.left = 91;
	m_rt.top = 1;
	m_rt.right = 41;
	m_rt.bottom = 60;

	if (!m_BackGround.Load(_T("../../data/kgca.bmp")))
	{
		return false;
	}
	if (!m_ColorBitmap.Load(_T("../../data/bitmap1.bmp")))
	{
		return false;
	}
	if (!m_MaskBitmap.Load(_T("../../data/bitmap2.bmp")))
	{
		return false;
	}
	return true;
}
///* Ternary raster operations */
//#define SRCCOPY             (DWORD)0x00CC0020 /* dest = source                   */
//#define SRCPAINT            (DWORD)0x00EE0086 /* dest = source OR dest           */
//#define SRCAND              (DWORD)0x008800C6 /* dest = source AND dest          */
//#define SRCINVERT           (DWORD)0x00660046 /* dest = source XOR dest          */
//#define SRCERASE            (DWORD)0x00440328 /* dest = source AND (NOT dest )   */
//#define NOTSRCCOPY          (DWORD)0x00330008 /* dest = (NOT source)             */
//#define NOTSRCERASE         (DWORD)0x001100A6 /* dest = (NOT src) AND (NOT dest) */
//#define MERGECOPY           (DWORD)0x00C000CA /* dest = (source AND pattern)     */
//#define MERGEPAINT          (DWORD)0x00BB0226 /* dest = (NOT source) OR dest     */
//#define PATCOPY             (DWORD)0x00F00021 /* dest = pattern                  */
//#define PATPAINT            (DWORD)0x00FB0A09 /* dest = DPSnoo                   */
//#define PATINVERT           (DWORD)0x005A0049 /* dest = pattern XOR dest         */
//#define DSTINVERT           (DWORD)0x00550009 /* dest = (NOT dest)               */
//#define BLACKNESS           (DWORD)0x00000042 /* dest = BLACK                    */
//#define WHITENESS           (DWORD)0x00FF0062 /* dest = WHITE */

void Sample::DrawBackGround()
{
	BitBlt(g_hOffScreenDC, 0, 0,
		m_BackGround.m_hBitInfo.bmWidth,
		m_BackGround.m_hBitInfo.bmHeight,
		m_BackGround.m_hMemDC,
		0, 0, SRCCOPY);

}
HBITMAP Sample::GetRotationBimap( HDC hdc, 
	float fAngle, int iWidth, int iHeight)
{	
	HBITMAP bitmap = CreateCompatibleBitmap(g_hScreenDC, iWidth,iHeight);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(m_RotationBitmapDC, bitmap);
	
	HBRUSH oldBrush = (HBRUSH)SelectObject(m_RotationBitmapDC, m_back);
	PatBlt(m_RotationBitmapDC, 0, 0, iWidth, iHeight, PATCOPY);

	int iOldMode = SetGraphicsMode(m_RotationBitmapDC, GM_ADVANCED);
	float fRadian = RadianToDegree(fAngle);
	float fCosine = cos(fRadian);
	float fSine = sin(fRadian);

	XFORM xForm;
	xForm.eM11 = fCosine;
	xForm.eM12 = fSine;
	xForm.eM21 =  -fSine;
	xForm.eM22 = fCosine;
	xForm.eDx = (iWidth / 2);
	xForm.eDy = (iHeight / 2);
	if (SetWorldTransform(m_RotationBitmapDC, &xForm) == FALSE)
	{
		return NULL;
	}

	BitBlt(m_RotationBitmapDC,
		    -(m_rt.right / 2),
		    -(m_rt.bottom / 2),
			m_rt.right,
			m_rt.bottom,
		    hdc, m_rt.left, m_rt.top, SRCCOPY);

	SelectObject(m_RotationBitmapDC, oldBitmap);
	
	//m_RotationBitmapDC을 공유하여 사용하기 위해서는 단위행렬로 해야한다.
	xForm.eM11 = 1;	xForm.eM12 = 0;
	xForm.eM21 = 0;	xForm.eM22 = 1;
	xForm.eDx = 0;	xForm.eDy = 0;
	SetWorldTransform(m_RotationBitmapDC, &xForm);
	SetGraphicsMode(m_RotationBitmapDC, iOldMode);

	return bitmap;
}
void Sample::DrawRotate(float fAngle)
{
	RECT rt;
	rt.left = 91;
	rt.top = 1;
	rt.right = 41;
	rt.bottom= 60;
	int iOffWidth = rt.right;
	int iOffHeight = rt.bottom;
	if (iOffWidth > iOffHeight) iOffHeight = iOffWidth;
	else iOffWidth = iOffHeight;	
	
	HBITMAP hMaskBitmap =GetRotationBimap(
		m_MaskBitmap.m_hMemDC, 
		fAngle, 
		iOffWidth, iOffHeight );
	HBITMAP hColorBitmap = GetRotationBimap(
		m_ColorBitmap.m_hMemDC,
		fAngle,
		iOffWidth, iOffHeight);

	
	HBITMAP hOldMask = (HBITMAP)SelectObject(m_hMaskMemDC,
		hMaskBitmap);
	HBITMAP hOldColor = (HBITMAP)SelectObject(m_hColorMemDC,
		hColorBitmap);

	if (BitBlt(g_hOffScreenDC, 400, 300,
		60,
		60,
		m_hMaskMemDC,
		0, 0, SRCAND)==FALSE)
	{
		return;
	};
	BitBlt(g_hOffScreenDC, 400, 300,
		60,
		60,
		m_hColorMemDC,
		0, 0, SRCINVERT);

	BitBlt(g_hOffScreenDC, 400, 300,
		60,
		60,
		m_hMaskMemDC,
		0, 0, SRCINVERT);

	SelectObject(m_hMaskMemDC, hOldMask);
	SelectObject(m_hColorMemDC, hOldColor);
	DeleteObject(hMaskBitmap);
	DeleteObject(hColorBitmap);	
}
void Sample::DrawObject()
{
	BitBlt(g_hOffScreenDC, 400, 300,
		41,
		60,
		m_MaskBitmap.m_hMemDC,
		91, 1, SRCAND);
	BitBlt(g_hOffScreenDC, 400, 300,
		41,
		60,
		m_ColorBitmap.m_hMemDC,
		91, 1, SRCINVERT);

	BitBlt(g_hOffScreenDC, 400, 300,
		41,
		60,
		m_MaskBitmap.m_hMemDC,
		91, 1, SRCINVERT);
}
bool Sample::Render()
{
	DrawBackGround();
	//DrawObject();
	DrawRotate(m_fAngle);
	return true;
}
bool Sample::Release()
{	
	DeleteDC(m_hMaskMemDC);
	DeleteDC(m_hColorMemDC);
	DeleteDC(m_RotationBitmapDC);
	DeleteObject(m_back);
	m_ColorBitmap.Release();
	m_MaskBitmap.Release();
	return true;
}
Sample::Sample()
{
	m_fAngle = 0.0f;
}


Sample::~Sample()
{
}


int WINAPI wWinMain(
	HINSTANCE hInstatnce,
	HINSTANCE hPrevInstatnce,
	LPWSTR   lpCmdLine,
	int      nCmdShow)
{
	Sample win;
	//win.SetRect(1024, 768);
	if (win.SetWindow(hInstatnce) == true)
	{
		win.Run();
	}
	return 0;
}