#include "Sample.h"

bool		(Sample::*FunCall)();

bool		UI()
{
	return true;
};
// 로그인, 타이틀화면
bool		Sample::Scene0()
{
	m_Lobby.m_fAlpha += g_fSecPerFrame * 50 * m_fDirection;
	if (m_Lobby.m_fAlpha > 255.0f)
	{
		m_Lobby.m_fAlpha = 255.0f;
		m_bGameStart = true;
	}
	if (m_Lobby.m_fAlpha < 0.0f)
	{
		m_Lobby.m_fAlpha = 0.0f;
		m_bGameStart = true;		
	}
	if (I_Input.KeyCheck(VK_F1) == KEY_HOLD)
	{
		m_Lobby.m_fAlpha-= g_fSecPerFrame * 100;
		if (m_Lobby.m_fAlpha < 0.0f) m_Lobby.m_fAlpha = 0.0f;

	}
	if (I_Input.KeyCheck(VK_F2) == KEY_HOLD)
	{
		m_Lobby.m_fAlpha += g_fSecPerFrame * 100;
		if (m_Lobby.m_fAlpha > 255.0f) m_Lobby.m_fAlpha = 255.0f;
	}
	if (m_fDirection < 0.0f )
	{
		m_Ground.Render();
	}
	m_Lobby.RenderAlpha();
	if (I_Input.KeyCheck(VK_ESCAPE) == KEY_PUSH)
	{
		m_fDirection = -1.0f;			
	}	
	return true;
};
// 로비
bool		Sample::Scene1()
{
	m_Ground.Render();
	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		if (m_bnpc[inpc])	m_npc[inpc].Render();
	}
	return true;
};
bool		Sample::Scene2()
{
	m_Hero.m_fAlpha += g_fSecPerFrame * 100 * m_fDirection;
	if (m_Hero.m_fAlpha > 255.0f)
	{
		m_Hero.m_fAlpha = 0.0f;
	}

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
		//if (m_bGameEnding == true)break;
		m_npc[inpc].Frame();
		if (m_bnpc[inpc] && I_Input.m_dwKeyState[VK_LBUTTON] == TRUE &&
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
	m_Ground.Render();
	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		if (m_bnpc[inpc])	m_npc[inpc].Render();
	}

	//m_Hero.Render();

	static float fAngle = 0;
	fAngle += g_fSecPerFrame * 100.0f;
	m_Hero.RotateBlt(fAngle);
	return true;
};
bool		Sample::Init()
{	
	m_Lobby.Init();
	m_Lobby.Load(m_hScreenDC,
		m_hOffScreenDC,
		NULL,
		L"../../data/kgca.bmp");
	RECT rtLobby = { 0, 0, 800, 600 };
	tPOINT posLobby = { 0, 0 };
	m_Lobby.SetRect(rtLobby, posLobby, false, 0.0f);


	m_Ground.Init();
	m_Ground.Load(m_hScreenDC,
		m_hOffScreenDC,
		NULL,
		L"../../data/kgca.bmp");
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

	FunCall = &Sample::Scene0;
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
	if (m_bGameStart == true)
	{
		FunCall = &Sample::Scene2;
	}
	if (m_bGameEnding == true)
	{
		m_bGameStart = false;
		m_bGameEnding = false;
		for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
		{
			m_bnpc[inpc] = true;
			RECT rt[2] = { 46, 62, 68, 79,
				115, 62, 37, 35 };
			tPOINT pos = { rand() % (m_rtClient.right - 400) + 200,
				rand() % (m_rtClient.bottom - 200) + 100 };
			m_npc[inpc].SetRect(rt[inpc % 2], pos, false, (rand() % 90) + 10.0f);
			m_npc[inpc].SetRect(rt[inpc % 2], pos, false, (rand() % 90) + 10.0f);
		}
		tPOINT pos = { 100, 100 };
		RECT rt = { 90, 1, 40, 60 };
		m_Hero.SetRect(rt, pos, true, 200.0f);
		m_fDirection = 1.0f;
		m_Lobby.m_fAlpha = 255.0f;
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
	m_bGameStart = false;
	m_fDirection = 1.0f;
}


Sample::~Sample()
{
}
//TCORE_RUN(SampleLib_0, 1024, 768);
TCORE_START;
// 윈도우를 생성한다.
if (sample.SetWindow(hInstance,
	L"SampleScene_0", 800, 600))
{
	
	
	// 생성된 윈도우를 가동한다.
	sample.Run();
}
TCORE_END;