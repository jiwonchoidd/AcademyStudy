#include "Sample.h"

bool		(Sample::*FunCall)();

bool		UI()
{
	return true;
};
// �α���, Ÿ��Ʋȭ��
bool		Sample::Scene0()
{
	m_Ground.Render();
	//MessageBox(m_hWnd, _T("##"), _T("��������"), MB_OK);
	return true;
};
// �κ�
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
	if (I_Input.KeyCheck(VK_HOME) == KEY_PUSH )
	{
		m_Sound.Stop(0);
	}
	if (I_Input.KeyCheck(VK_END) == KEY_PUSH )
	{
		m_Sound.Paused(0);
	}
	if (I_Input.KeyCheck(VK_INSERT) == KEY_HOLD)
	{
		m_Sound.Volume(0, g_fSecPerFrame*3.0f, true);
	}
	if (I_Input.KeyCheck(VK_DELETE) == KEY_HOLD)
	{
		m_Sound.Volume(0, g_fSecPerFrame*3.0f, false);
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

	// �浹 ����
	// ���콺 ��ǥ
	POINT cursor;
	GetCursorPos(&cursor); // ȭ����ǥ��
	ScreenToClient(m_hWnd, &cursor);// Ŭ���̾�Ʈ ��ǥ��

	m_Hero.Frame();

	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		//if (m_bGameEnding == true)break;
		m_npc[inpc].Frame();
		if (m_bnpc[inpc] && I_Input.KeyCheck(VK_LBUTTON) == KEY_PUSH &&
			m_Collision.RectInPt(m_npc[inpc].m_rtCollision, cursor))
		{
			m_bnpc[inpc] = false;
			m_Sound.Play(2, false);
		}
		if (m_bnpc[inpc] &&
		m_Collision.RectInRect(m_npc[inpc].m_rtCollision,
		m_Hero.m_rtCollision))
		{
				m_bnpc[inpc] = false;
				m_bGameEnding = true;
				m_Sound.Play(1, true);
		//MessageBox(m_hWnd, _T("##"), _T("��������"), MB_OK);
		}
		if (m_bnpc[inpc] &&
			m_Collision.SphereInSphere(m_npc[inpc].m_rtCollision,
				m_Hero.m_rtCollision))
		{
			m_bnpc[inpc] = false;
			m_bGameEnding = true;
			m_Sound.Play(1, true);
			//MessageBox(m_hWnd, _T("##"), _T("��������"), MB_OK);
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
	m_Sound.Init();
	// ���� �ε� �� ����
	m_Sound.Load("../../data/Mylove.mp3", true);
	m_Sound.Load("../../data/PianoSound_04.mp3");
	m_Sound.Load("../../data/PianoSound_00.mp3");

	m_Ground.Init();
	m_Ground.Load(m_hScreenDC,
		m_hOffScreenDC,
		NULL,
		L"../../data/ground.bmp");
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
	DrawDebug(m_Sound.m_szBuffer, 0, 200);
	return true;
};
bool		Sample::Frame()
{
	if (m_bGameEnding == true)
	{
		//FunCall = &Sample::Scene0;
	}
	m_Sound.Frame();
	return true;
};
bool		Sample::Release()
{
	m_Hero.Release();
	m_Sound.Release();
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
// �����츦 �����Ѵ�.
if (sample.SetWindow(hInstance,
	L"SampleScene_0", 800, 600, WS_POPUPWINDOW))
{
	
	
	// ������ �����츦 �����Ѵ�.
	sample.Run();
}
TCORE_END;