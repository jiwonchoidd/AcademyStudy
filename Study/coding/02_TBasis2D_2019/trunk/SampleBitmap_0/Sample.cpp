#include "Sample.h"

bool Sample::PreRender()
{
	SelectObject(m_hOffScreenDC, m_hbrBlack);
	PatBlt(m_hOffScreenDC, 0, 0,
		m_rtClient.right, m_rtClient.bottom,
		PATCOPY);
	return true;
}
bool		Sample::Init()
{
	COLORREF bkColor = RGB(0, 0, 255);
	m_hbrBlack = CreateSolidBrush(bkColor);
	//m_hbrOld =	(HBRUSH)SelectObject(m_hOffScreenDC, m_hbrBlack);

	m_Bitmap.Init();
	m_Bitmap.Set(m_hScreenDC);
	m_Bitmap.Load(L"../../data/bitmap1.bmp");
	HBRUSH hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(
		this->m_hOffScreenDC, hBrush);

	m_rtObj = { 90, 1, 40, 60};
	return true;
};
bool		Sample::Render()
{
	m_Bitmap.Draw(m_hOffScreenDC,m_ObjPos,
		m_rtObj, 2);

	tPOINT pos = m_ObjPos;
	pos.x += 100;
	m_Bitmap.Draw(m_hOffScreenDC, pos,
		m_rtObj, 1);

	pos.x += 100;
	m_Bitmap.Draw(m_hOffScreenDC, pos,
		m_rtObj, 4);

	return true;
};
bool		Sample::Frame()
{
	if( I_Input.KeyCheck(VK_LEFT) == KEY_PUSH ||
		I_Input.KeyCheck(VK_LEFT) == KEY_HOLD  )
	{
		m_ObjPos.x -= g_fSecPerFrame * 100.0f;
	}
	if (I_Input.KeyCheck(VK_RIGHT) == KEY_PUSH ||
		I_Input.KeyCheck(VK_RIGHT) == KEY_HOLD)
	{
		m_ObjPos.x+= g_fSecPerFrame * 100.0f;
	}
	if (I_Input.KeyCheck(VK_UP) == KEY_PUSH ||
		I_Input.KeyCheck(VK_UP) == KEY_HOLD)
	{
		m_ObjPos.y -= g_fSecPerFrame * 100.0f;
	}
	if (I_Input.KeyCheck(VK_DOWN) == KEY_PUSH ||
		I_Input.KeyCheck(VK_DOWN) == KEY_HOLD)
	{
		m_ObjPos.y += g_fSecPerFrame * 100.0f;
	}
	return true;
};
bool		Sample::Release()
{
	m_Bitmap.Release();
	return true;
};

Sample::Sample()
{
	m_ObjPos.x = 100;
	m_ObjPos.y = 100;
}


Sample::~Sample()
{
}
