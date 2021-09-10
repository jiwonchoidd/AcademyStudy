#include "Sample.h"
bool		Sample::Init()
{
	m_Ground.Init();
	m_Ground.Load(m_hScreenDC,m_hOffScreenDC,
		NULL,L"../../data/ground.bmp");
	RECT rtGround = { 0, 0, 800, 600 };
	tPOINT posGround = { 0, 0 };
	m_Ground.SetRect(rtGround, posGround, false, 0.0f);

	m_Hero.Init();
	m_Hero.Load(m_hScreenDC, 
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

	if (m_Projectile.Load(m_hScreenDC, m_hOffScreenDC, L"../../data/bitmap2.bmp",
									L"../../data/bitmap1.bmp"))
	{
		RECT rt[2] = {  276, 1, 8, 24,
						115, 62, 37, 35 };
		tPOINT pos = { m_Hero.m_ObjPos.x,
						m_Hero.m_ObjPos.y };
		m_Projectile.SetRect(rt[0], pos, false, 100);

	/*	RECT rtSrc, rtDesk;
		rtSrc.left = 276;
		rtSrc.top = 1;
		rtDesk.left = m_Hero.m_ObjPos.x;
		rtDesk.top = m_Hero.m_ObjPos.y;
		rtDesk.right = 8;
		rtDesk.bottom = 24;
		m_Projectile.SetRect(rtSrc, rtDesk);*/
	}
	return true;
};
bool		Sample::Render()
{
	m_Ground.Render();
	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		if(m_bnpc[inpc])	m_npc[inpc].Render();
	}
	m_Hero.Render();

	list<TProjectile>::iterator iter;
	for (iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end(); iter++)
	{
		m_Projectile.m_ObjPos.x = (*iter).pos.x;
		m_Projectile.m_ObjPos.y = (*iter).pos.y;
		m_Projectile.DrawColorKey();
	}
	return true;
};
bool		Sample::Frame()
{
	if( I_Input.KeyCheck(VK_LEFT) == KEY_PUSH ||
		I_Input.KeyCheck(VK_LEFT) == KEY_HOLD  )
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

	m_Projectile.Frame();
	if (g_ActionInput.bLeftClick == KEY_PUSH)
	{
		TProjectile item;
		item.fLifeTime = 2.0f;
		item.rtDesk = m_Projectile.m_rtObj;
		item.SetPos(m_Hero.m_ObjPos.x, m_Hero.m_ObjPos.y);
		m_ProjectileList.insert(m_ProjectileList.end(),	item);
		//I_SoundMgr.GetPtr(2)->PlayEffect();
	}
	list<TProjectile>::iterator iter;
	for (iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end(); )
	{
		(*iter).fLifeTime -= g_fSecPerFrame;
		(*iter).pos.y -= g_fSecPerFrame * 500.0f;
		(*iter).SetPos((*iter).pos.x, (*iter).pos.y);

		if ((*iter).fLifeTime < 0.0f)
		{
			iter = m_ProjectileList.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		m_npc[inpc].Frame();
		if (g_ActionInput.bLeftClick == KEY_PUSH &&
			RectInPt(m_npc[inpc].m_rtCollision, cursor))
		{
			m_bnpc[inpc] = false;	
		}
		// Collision
		list<TProjectile>::iterator iter;
		for (iter = m_ProjectileList.begin();
			iter != m_ProjectileList.end();
			iter++)
		{
			if (RectInRect( m_npc[inpc].m_rtCollision,
							(*iter).rtCollision))
			{
				m_npc[inpc].m_bDead = true;
				break;
			}
		}

		/*if (m_bnpc[inpc] && 
		RectInRect(m_npc[inpc].m_rtCollision,
						m_Hero.m_rtCollision))
		{
			m_bnpc[inpc] = false;	
			m_bGameEnding = true;
			MessageBox(m_hWnd, _T("##"), _T("게임종료"), MB_OK);
		}*/
		if (m_bnpc[inpc] && 
			SphereInSphere(m_npc[inpc].m_rtCollision,
			m_Hero.m_rtCollision))
		{
			m_bnpc[inpc] = false;
			m_bGameEnding = true;
			//MessageBox(m_hWnd, _T("##"), _T("게임종료"), MB_OK);
		}
	}
	
	
	return true;
};
bool		Sample::RectInPt(RECT rt, POINT pt)
{
	if (rt.left <= pt.x && rt.right >= pt.x)
	{
		if (rt.top <= pt.y && rt.bottom >= pt.y)
		{
			return true;
		}
	}
	return false;
}
bool		Sample::SphereInSphere(RECT rtDesk, RECT rtSrc)
{
	// 가로
	POINT  cDesk, cSrc, Radius, DeskRadius, SrcRadius;
	cDesk.x = (rtDesk.left + rtDesk.right) / 2;
	cDesk.y = (rtDesk.top + rtDesk.bottom) / 2;	
	cSrc.x = (rtSrc.left + rtSrc.right) / 2;
	cSrc.y = (rtSrc.top + rtSrc.bottom) / 2;

	DeskRadius.x = (rtDesk.right - rtDesk.left) / 2;
	DeskRadius.y = (rtDesk.bottom - rtDesk.top) / 2;
	SrcRadius.x = (rtSrc.right - rtSrc.left) / 2;
	SrcRadius.y = (rtSrc.bottom - rtSrc.top) / 2;
	LONG  deskRadius, srcRedius;
	deskRadius = max(DeskRadius.x, DeskRadius.y);
	srcRedius = max(SrcRadius.x, SrcRadius.y);

	Radius.x = abs(cDesk.x - cSrc.x);
	Radius.y = abs(cDesk.y - cSrc.y);
	
	LONG Distatnce = sqrt((Radius.x * Radius.x + Radius.y * Radius.y));
	if (Distatnce <= (deskRadius + srcRedius))
	{
		return true;
	}
	return false;
}
bool		Sample::RectInRect(RECT rtDesk, RECT rtSrc)
{
	// 가로
	POINT  cDesk, cSrc, Radius, DeskRadius, SrcRadius;
	cDesk.x = (rtDesk.left + rtDesk.right) / 2;
	cDesk.y = (rtDesk.top + rtDesk.bottom) / 2;
	cSrc.x = (rtSrc.left + rtSrc.right) / 2;
	cSrc.y = (rtSrc.top + rtSrc.bottom) / 2;
	Radius.x = abs(cDesk.x - cSrc.x);
	Radius.y = abs(cDesk.y - cSrc.y);
	DeskRadius.x = (rtDesk.right - rtDesk.left)/2;
	DeskRadius.y = (rtDesk.bottom - rtDesk.top) / 2;
	SrcRadius.x = (rtSrc.right - rtSrc.left) / 2;
	SrcRadius.y = (rtSrc.bottom - rtSrc.top) / 2;
	if (Radius.x <= (DeskRadius.x + SrcRadius.x) &&
		Radius.y <= (DeskRadius.y + SrcRadius.y))
	{
		return true;
	}
	return false;
}
bool		Sample::Release()
{
	m_Hero.Release();
	m_Projectile.Release();
	m_Ground.Release();
	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		m_npc[inpc].Release();
	}
	return true;
};

Sample::Sample()
{
	m_bGameEnding = false;
}


Sample::~Sample()
{
}
