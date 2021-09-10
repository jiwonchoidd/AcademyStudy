//#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"

bool		(Sample::*FunCall)();

bool		UI()
{
	return true;
};
bool Sample::Opening()
{
	POINT pos;
	pos.x = m_Ground.m_ObjPos.x;
	pos.y = m_Ground.m_ObjPos.y;
	RECT rt = m_Ground.m_rtObj;

	m_fScrollX += (cosf(timeGetTime()) * 0.5f + 0.5f) * 0.5f;
	float wOffsetX = max(0.0f, rt.right - m_fScrollX);
	if (m_fScrollX >= rt.right)
	{
		m_fScrollX = 0;
	}	

	BitBlt(m_hOffScreenDC, pos.x + m_fScrollX, pos.y,
		rt.right, rt.bottom,
		m_Ground.m_pBitmap->m_hMemDC, rt.left, rt.top,
		SRCCOPY);

	BitBlt(m_hOffScreenDC, pos.x, pos.y,
		m_fScrollX, rt.bottom,
		m_Ground.m_pBitmap->m_hMemDC,
		rt.left + wOffsetX, rt.top,
		SRCCOPY);

	float fY = pos.y + m_fScrollX;
	BitBlt(m_hOffScreenDC, pos.x, fY,
		rt.right, rt.bottom,
		m_Ground.m_pBitmap->m_hMemDC, rt.left, rt.top,
		SRCCOPY);
	BitBlt(m_hOffScreenDC, pos.x, pos.y,
		rt.right, rt.bottom - fY,
		m_Ground.m_pBitmap->m_hMemDC, rt.left, rt.top,
		SRCCOPY);

	if (fY >= rt.bottom)
	{
		FunCall = &Sample::Scene2;
		return true;
	}

	return false;
}
///////////////////////////////////////////////////////
// 텍스트 파일 라인 입력
///////////////////////////////////////////////////////   
bool Sample::GameDataLoad(TCHAR* pszLoad)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	int iNumSprite = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, pszLoad, _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, (unsigned int)_countof(pTemp), &iNumSprite);
	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d"), pTemp, (unsigned int)_countof(pTemp), &iNumFrame);
		//m_rtSpriteList[iCnt].resize(iNumFrame);

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, (unsigned int)_countof(pTemp),
										&rt.left, &rt.top, &rt.right, &rt.bottom);
			m_rtSpriteList[iCnt].push_back(rt);
		}		
	}
	fclose(fp_src);
	return true;
}
// 로그인, 타이틀화면
bool		Sample::Scene0()
{
	m_Ground.Render();	
	int iRet = MessageBox(m_hWnd, _T("처음부터 다시 시작 합니다.^^~"), _T("게임종료"), MB_OK);
	if (iRet == MB_OKCANCEL)
	{
		FunCall = &Sample::Opening;
		m_fScrollX = 0.0f;
	}
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
		if (g_ActionInput.bLeftClick == TRUE &&
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
	GameDataLoad(L"RectBitmap1.txt");
	m_Ground.Init();
	m_Ground.Load(m_hScreenDC,
		m_hOffScreenDC,		
		L"../../data/inForm.bmp");
	RECT rtGround = { 0, 0, 800, 600 };
	tPOINT posGround = { 0, 0 };
	m_Ground.SetRect(rtGround, posGround);

	m_Hero.Init();
	m_Hero.Load(	m_hScreenDC, 
				m_hOffScreenDC,				
				L"../../data/bitmap1.bmp",
				L"../../data/bitmap2.bmp" );
	RECT rt = { 90, 1, 40, 60 };
	tPOINT pos = { 100, 100 };
	m_Hero.SetRect(rt, pos);
	m_Hero.SetSpeed(200.0f);

	srand((unsigned)time(NULL));
	for (int inpc = 0; inpc < g_iMaxNPC; inpc++)
	{
		m_bnpc[inpc] = true;
		m_npc[inpc].Init();
		m_npc[inpc].Load(m_hScreenDC,
			m_hOffScreenDC,			
			L"../../data/bitmap1.bmp",
			L"../../data/bitmap2.bmp" );
		RECT rt[2] = {  46, 62, 68, 79, 
						115, 62, 37, 35 };
		tPOINT pos = {	rand() % (m_rtClient.right-400) + 200,
						rand() % (m_rtClient.bottom-200) + 100};
		m_npc[inpc].SetRect(rt[inpc%2], pos);
		m_npc[inpc].SetRect(rt[inpc%2],pos);
		m_npc[inpc].SetSpeed((rand() % 90) + 10.0f);
	}

	FunCall = &Sample::Opening;
	//FunCall = Scene0;
	

	///////////////////////////////////////////////////////////
	// Sprite Type  = A
	///////////////////////////////////////////////////////////
	I_Sprite.SetDevice( g_hScreenDC, g_hOffScreenDC);
	INT iIndex = I_Sprite.Add( L"TypeA", 
								L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	xSprite* pSprite = I_Sprite.GetPtr(iIndex);
	pSprite->SetRectArray(m_rtSpriteList[0],1.0f);
	///////////////////////////////////////////////////////////
	// Sprite Type  = B(Bomb)
	///////////////////////////////////////////////////////////
	iIndex = I_Sprite.Add(L"TypeB", L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	pSprite = I_Sprite.GetPtr(iIndex);
	pSprite->SetRectArray(m_rtSpriteList[1], 0.7f);
	///////////////////////////////////////////////////////////
	// Sprite Type  = B(Clash)
	///////////////////////////////////////////////////////////
	iIndex = I_Sprite.Add(L"TypeC", L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	pSprite = I_Sprite.GetPtr(iIndex);
	pSprite->SetRectArray(m_rtSpriteList[2],0.3f);
	///////////////////////////////////////////////////////////
	// Sprite Type  = C
	///////////////////////////////////////////////////////////
	/////*iIndex = I_Sprite.Add(L"TypeC");
	////pSprite = I_Sprite.GetPtr(iIndex);
	////pSprite->SetRect(rtSprite[1], posSprite);

	////TCHAR_STRING_VECTOR FileList;
	////for (int iTex = 0; iTex < 10; iTex++)
	////{
	////	TCHAR pBuffer[50];
	////	if (iTex < 10)
	////		_stprintf_s(pBuffer, L"../../data/shine%d.bmp", iTex);
	////	else
	////		_stprintf_s(pBuffer, L"../../data/shine0%d.bmp", iTex);
	////	FileList.push_back(pBuffer);
	////}
	////pSprite->SetBitmapArray(FileList);
	////pSprite->SetAttribute(2.0f);*/

	// 파티클 최대갯수를 지정한다.
	POINT vScale;
	vScale.x = 1;
	vScale.y = 1;
	if (!m_ParticleSystem.Create(g_hScreenDC,100, TBASIS_PI*2.0f, 30.0f, vScale))
	{
		return false;
	}
	m_ParticleSystem.SetScale(10.0f, 10.0f);
	m_ParticleSystem.SetAddRadius(150.0f);

	return true;
};
bool		Sample::Render()
{	
	(this->*FunCall)();	
	m_ParticleSystem.Render();
	return true;
};
bool		Sample::Frame()
{
	if (m_bGameEnding == true)
	{
		FunCall = &Sample::Scene0;
		m_bGameEnding = false;
	}
	m_fSumTimer += m_Timer.m_fSecondPerFrame;
	float fRadius = 1.0f;

	if (I_Input.KeyCheck('1')==KEY_PUSH)
	{
		m_dwEffectType++;
		if (m_dwEffectType > 13) m_dwEffectType = 0;
		m_ParticleSystem.ResetCounter();
		m_fSumTimer = 0.0f;
	}
	if (I_Input.KeyCheck('2') == KEY_PUSH)
	{
		m_dwEffectType--;
		if (m_dwEffectType < 0) m_dwEffectType = 12;
		m_ParticleSystem.ResetCounter();
		m_fSumTimer = 0.0f;
	}


	switch (m_dwEffectType)
	{
	case 1: // 세잎 장미
		fRadius = sinf(m_fSumTimer / 2); break;
	case 2: // 네잎장미
		fRadius = sinf(2 * m_fSumTimer); break;
	case 3: // 별
		fRadius = sinf(4 * m_fSumTimer); break;

	case 4:  // Cardioid( 심장형 곡선 좌 )
		fRadius = 1 + cosf(m_fSumTimer); break;
	case 5:  // Cardioid( 심장형 곡선 우)
		fRadius = 1 - cosf(m_fSumTimer); break;
	case 6:  // Cardioid( 심장형 곡선 상)
		fRadius = 1 - sinf(m_fSumTimer); break;
	case 7:  // Cardioid( 심장형 곡선 상)
		fRadius = 1 + sinf(m_fSumTimer); break;

	case 8:  // 파형 가로
		fRadius = 1 + cosf(2 * m_fSumTimer); break;
	case 9:  // 파형 세로
		fRadius = 1 - cosf(2 * m_fSumTimer); break;
	case 10: // 트랙 ( 가로 )
		fRadius = 2 + cosf(2 * m_fSumTimer); break;
	case 11: // 트랙( 세로 ) 
		fRadius = 2 - cosf(2 * m_fSumTimer); break;
	case 12:// 별
		fRadius = tanf(m_fSumTimer); break;
	default:
		break;
	}
	m_ParticleSystem.SetRadius(fRadius);
	m_ParticleSystem.Frame( m_fSumTimer, m_Timer.m_fSecondPerFrame);
	return true;
};
bool		Sample::Release()
{
	m_Hero.Release();
	m_ParticleSystem.Release();
	return true;
}

Sample::Sample()
{
	m_bGameEnding = false;
	m_dwEffectType = 0;
	m_fScrollX = 0.0f;
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