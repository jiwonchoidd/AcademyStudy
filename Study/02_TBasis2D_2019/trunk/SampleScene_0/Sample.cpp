#include "Sample.h"

bool		(Sample::*FunCall)();

bool		UI()
{
	return true;
};
// 로그인, 타이틀화면
bool		Sample::Scene0()
{
	m_Ground.Render();
	//MessageBox(m_hWnd, _T("##"), _T("게임종료"), MB_OK);
	return true;
};
// 로비
bool		Sample::Scene1()
{
	static float fAngle = 0;
	fAngle += g_fSecPerFrame * 100.0f;
	m_Ground.Render();
	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		if (m_bnpc[inpc])	m_npc[inpc].Render();
	}
	return true;
};
bool		Sample::Scene2()
{
	if (I_Input.KeyCheck(VK_LEFT) == KEY_PUSH ||
		I_Input.KeyCheck(VK_LEFT) == KEY_HOLD)
	{
		m_Hero.Left();
	}
	if (I_Input.KeyCheck(VK_RIGHT) == KEY_PUSH ||
		I_Input.KeyCheck(VK_RIGHT) == KEY_HOLD)
	{
		m_Hero.Right();
	}
	if (I_Input.KeyCheck(VK_UP) == KEY_PUSH ||
		I_Input.KeyCheck(VK_UP) == KEY_HOLD)
	{
		m_Hero.Up();
	}
	if (I_Input.KeyCheck(VK_DOWN) == KEY_PUSH ||
		I_Input.KeyCheck(VK_DOWN) == KEY_HOLD)
	{
		m_Hero.Down();
	}

	// 충돌 판정
	// 마우스 좌표
	POINT cursor;
	GetCursorPos(&cursor); // 화면좌표계
	ScreenToClient(m_hWnd, &cursor);// 클라이언트 좌표계

	m_Hero.Frame();

	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		m_npc[inpc].Frame();
		if (I_Input.m_dwMouseState[0] == TRUE &&
			m_Collision.RectInPt(m_npc[inpc].m_rtCollision, cursor))
		{
			m_bnpc[inpc] = false;
		}
		if (m_bnpc[inpc] &&
		m_Collision.RectInRect(m_npc[inpc].m_rtCollision,
		m_Hero.m_rtCollision))
		{
				m_bnpc[inpc] = false;
				m_bGameEnding = true;
		//MessageBox(m_hWnd, _T("##"), _T("게임종료"), MB_OK);
		}
		if (m_bnpc[inpc] &&
			m_Collision.SphereInSphere(m_npc[inpc].m_rtCollision,
				m_Hero.m_rtCollision))
		{
			m_bnpc[inpc] = false;
			m_bGameEnding = true;
			//MessageBox(m_hWnd, _T("##"), _T("게임종료"), MB_OK);
		}
	}
	static float fAngle = 0;
	fAngle += g_fSecPerFrame * 100.0f;
	m_Ground.Render();
	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		if (m_bnpc[inpc])	m_npc[inpc].Render();
	}
	m_Hero.Render();
	return true;
};
bool		Sample::Init()
{	
	m_Ground.Init();
	m_Ground.Load(m_hScreenDC,
		m_hOffScreenDC,
		NULL,
		L"../../data/kgca01.bmp");
	RECT rtGround = { 0, 0, 800, 600 };
	tPOINT posGround = { 0, 0 };
	m_Ground.SetRect(rtGround, posGround, false, 0.0f);

	m_Hero.Init();
	m_Hero.Load(	m_hScreenDC, 
				m_hOffScreenDC,
				L"../../data/bitmap2.bmp",
				L"../../data/bitmap1.bmp");
	RECT rt = { 90, 1, 40, 60 };
	tPOINT pos = { 100, 100 };
	m_Hero.SetRect(rt, pos, true, 200.0f);

	srand((unsigned)time(NULL));
	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		m_bnpc[inpc] = true;
		m_npc[inpc].Init();
		m_npc[inpc].Load(m_hScreenDC,
			m_hOffScreenDC,
			L"../../data/bitmap2.bmp",
			L"../../data/bitmap1.bmp");
		RECT rt[2] = {  46, 62, 68, 79, 
						115, 62, 37, 35 };
		tPOINT pos = {	rand() % (m_rtClient.right-400) + 200,
						rand() % (m_rtClient.bottom-200) + 100};
		m_npc[inpc].SetRect(rt[inpc%2], pos, false, (rand()%90)+10.0f);
		m_npc[inpc].SetRect(rt[inpc%2],pos, false, (rand()%90) + 10.0f);
	}

	FunCall = &Sample::Scene2;
	//FunCall = Scene0;
	return true;
};
bool		Sample::Render()
{	
	(this->*FunCall)();	
	return true;
};
bool		Sample::Frame()
{
	if (m_bGameEnding == true)
	{
		FunCall = &Sample::Scene0;
	}
	return true;
};
bool		Sample::Release()
{
	m_Hero.Release();
	return true;
};

Sample::Sample()
{
	m_bGameEnding = false;
}


Sample::~Sample()
{
}
//TCORE_RUN(SampleLib_0, 1024, 768);
TCORE_START;
// 윈도우를 생성한다.
if (sample.SetWindow(hInstance,
	L"SampleScene_0", 800, 600, WS_POPUPWINDOW))
{
	
	
	// 생성된 윈도우를 가동한다.
	sample.Run();
}
TCORE_END;