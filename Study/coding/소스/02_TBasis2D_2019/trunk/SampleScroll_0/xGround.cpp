#include "xGround.h"
#include "TInput.h"
bool  xGround::SideScrolling()
{
	static float fScroll = 0.0f;
	fScroll += g_fSecPerFrame * 200;
	if (fScroll > m_rtObj.right)
	{
		fScroll = 0;
	}
	float left = m_rtObj.right - fScroll;
	// 좌측 스크롤
	/*BitBlt(g_hOffScreenDC,
		0, 0,
		g_rtClient.right, g_rtClient.bottom,
		m_pBitmap->m_hMemDC, 0, fScroll, SRCCOPY);
	BitBlt(g_hOffScreenDC,
		0, left,
		g_rtClient.right, g_rtClient.bottom,
		m_pBitmap->m_hMemDC, 0, 0, SRCCOPY);*/
	// 아래쪽 스크롤
	BitBlt(g_hOffScreenDC,
		0, 0,
		g_rtClient.right, g_rtClient.bottom,
		m_pBitmap->m_hMemDC, fScroll, 0, SRCCOPY);
	BitBlt(g_hOffScreenDC,
		left, 0,
		g_rtClient.right, g_rtClient.bottom,
		m_pBitmap->m_hMemDC, 0, 0, SRCCOPY);
	return true;
}
bool xGround::AutoDown()
{
	m_fScrollValue += g_fSecPerFrame * 100;
	if (m_fScrollValue > m_rtObj.bottom) m_fScrollValue = 0;

	RECT rtB = m_rtObj;
	RECT rtA = m_rtObj;
	rtA.top = m_rtObj.bottom - m_fScrollValue;
	rtA.bottom = m_fScrollValue;
	m_pBitmap->Draw(0, 0, rtA, SRCCOPY);
	rtB.bottom = rtB.bottom - m_fScrollValue;
	m_pBitmap->Draw(0, m_fScrollValue, rtB, SRCCOPY);

	/*SetROP2(g_hOffScreenDC, R2_NOTXORPEN);
	Rectangle(g_hOffScreenDC,
		0, 0,
		rtA.right,
		rtA.bottom );
	Rectangle(g_hOffScreenDC,
		0, m_fScrollValue,
		800,
		600);*/
		//m_bk.Render();
	return true;
}
bool xGround::AutoUp()
{
	m_fScrollValue -= g_fSecPerFrame * 100*2;
	if (m_fScrollValue > m_rtObj.bottom) m_fScrollValue = 0;
	if (m_fScrollValue < 0) m_fScrollValue = m_rtObj.bottom;

	RECT rtB = m_rtObj;
	RECT rtA = m_rtObj;
	rtA.top = m_rtObj.bottom - m_fScrollValue;
	rtA.bottom = m_fScrollValue;
	m_pBitmap->Draw(0, 0, rtA, SRCCOPY);
	rtB.bottom = rtB.bottom - m_fScrollValue;
	m_pBitmap->Draw(0, m_fScrollValue, rtB, SRCCOPY);

	/*SetROP2(g_hOffScreenDC, R2_NOTXORPEN);
	Rectangle(g_hOffScreenDC,
		0, 0,
		rtA.right,
		rtA.bottom );
	Rectangle(g_hOffScreenDC,
		0, m_fScrollValue,
		800,
		600);*/
		//m_bk.Render();
	return true;
}
bool xGround::InputTopDown()
{
	if (I_Input.KeyCheck(VK_UP) == KEY_HOLD)
	{
		AutoDown();
	}
	if (I_Input.KeyCheck(VK_DOWN) == KEY_HOLD)
	{
		AutoUp();
	}

	RECT rtB = m_rtObj;
	RECT rtA = m_rtObj;
	rtA.top = m_rtObj.bottom - m_fScrollValue;
	rtA.bottom = m_fScrollValue;
	m_pBitmap->Draw(0, 0, rtA, SRCCOPY);
	rtB.bottom = rtB.bottom - m_fScrollValue;
	m_pBitmap->Draw(0, m_fScrollValue, rtB, SRCCOPY);

	/*SetROP2(g_hOffScreenDC, R2_NOTXORPEN);
	Rectangle(g_hOffScreenDC,
		0, 0,
		rtA.right,
		rtA.bottom );
	Rectangle(g_hOffScreenDC,
		0, m_fScrollValue,
		800,
		600);*/
		//m_bk.Render();
	return true;
}

xGround::xGround()
{
	m_fScrollValue = 0.0f;
}


xGround::~xGround()
{
}
