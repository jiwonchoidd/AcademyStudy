#include "Sample.h"
//#include "TSys.h"
#include "TBitmapMgr.h"
bool	 Sample::Init()
{
	m_BackGround.SetPos(0, 0, 0.0f);
	m_BackGround.SetRect(0, 0, 800, 600);
	m_Hero.SetPos(100, 100, 100.0f);
	m_Hero.SetRect(90, 1, 42, 60);

	m_BackGround.Load(m_hScreenDC,
		m_hOffScreenDC, L"../../data/bk.bmp");
	m_Hero.Load(m_hScreenDC,
		m_hOffScreenDC, L"../../data/Bitmap1.bmp");

	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].SetPos(
			rand() % g_rtClient.right/2,
			rand() % g_rtClient.bottom/2,
			50.0f + rand() % 100);
		m_Object[iObj].SetRect(46, 62, 68, 79);
		m_Object[iObj].Load(m_hScreenDC,
			m_hOffScreenDC, L"../../data/Bitmap1.bmp");

	}
	return true;
};
bool	 Sample::Frame() 
{
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].Frame();
	}

	if (m_Input.KeyCheck('W') == KEY_HOLD)
	{
		m_Hero.Up();
	}
	if (m_Input.KeyCheck('S') == KEY_HOLD)
	{
		m_Hero.Down();
	}
	if (m_Input.KeyCheck('A') == KEY_HOLD)
	{
		m_Hero.Left();
	}
	if (m_Input.KeyCheck('D') == KEY_HOLD)
	{
		m_Hero.Right();
	}
	/*if (m_Input.KeyCheck(VK_SPACE) == KEY_HOLD)
	{
		m_Hero.SetSpeed(30.0f);
	}
	else
	{
		m_Hero.SetSpeed(-100.0f);
	}*/
	return true;
};
bool	 Sample::Render() {
	m_BackGround.Draw();
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].Render();
	}
	m_Hero.Render();
	return true;
};
bool	 Sample::Release() 
{	
	TBitmapMgr::GetInstance().Release();
	m_BackGround.Release();
	m_Hero.Release();
	for (int iObj = 0; iObj < MAX_OBJECT; iObj++)
	{
		m_Object[iObj].Release();
	}
	return true;
};

Sample::Sample()
{
	m_iX = 0;
	m_iY = 0;
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
	if (win.SetWindow(hInstatnce) == true)
	{
		win.Run();
	}
	return 0;
}