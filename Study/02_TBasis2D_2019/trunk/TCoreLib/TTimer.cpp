#include "TTimer.h"
#include "TScriptMgr.h"

int			g_iFPS;// 1초당 프레임 개수
float		g_fSecPerFrame; // 1프레임의 경과시간
float		g_fAccumulation;// 실행된 경과 시간
// 클래스 멤버 변수 초기화
bool		TTimer::Init() 
{ 
	m_dwBeforeTick = timeGetTime();
	ZeroMemory(m_csBuffer, sizeof(TCHAR)* MAX_PATH);
	return true; 
};
// 매 프레임에서 계산을 담당한다.
bool		TTimer::Frame() 
{ 
	// 1000 == 1초(winmm.lib)
	DWORD dwCurrentTick = timeGetTime();
	DWORD dwElapseTick = dwCurrentTick - m_dwBeforeTick;
	// 디버깅(블랙포인터) 중과 같은 경우에도 프레임 고정값으로 제한한다.
	if (dwElapseTick > g_dwStepTick)
	{
		dwElapseTick = g_dwStepTick;
	}

	g_fSecPerFrame = m_fSecondPerFrame = dwElapseTick / 1000.0f;
	g_fAccumulation = m_fAccumulation += m_fSecondPerFrame;
	m_fFrameTime += m_fSecondPerFrame;

	if (m_fFrameTime > 1.0f)
	{
		g_iFPS = m_iFPS = m_dwFrameCnt;
		m_dwFrameCnt = 0;
		m_fFrameTime -= 1.0f;
		//_CRT_SECURE_NO_WARNINGS
		//_stprintf_s(m_csBuffer, L"Timer:[%10.4f] FPS:[%d] %f\n",
		//	m_fAccumulation, m_iFPS, m_fSecondPerFrame);
		//OutputDebugString(m_csBuffer);
	}

	m_dwFrameCnt++;
	m_dwBeforeTick = dwCurrentTick;
	return true; 
};
// 매 프레임에서 드로우를 담당한다.
bool		TTimer::Render() 
{ 
	if (I_ScriptManager.m_TimerObj.size())
	{
		int iMinute = m_fAccumulation / 60;
		int iSecond = (int)m_fAccumulation % 60;
		I_ScriptManager.m_TimerObj[iMinute /10].Draw(300, 0, SRCAND);
		I_ScriptManager.m_TimerObj[iMinute %10].Draw(350, 0, SRCAND);

		I_ScriptManager.m_TimerObj[iSecond/10].Draw(400, 0, SRCAND);
		I_ScriptManager.m_TimerObj[iSecond%10].Draw(450, 0, SRCAND);
	}
	_stprintf_s(m_csBuffer, L"Timer:[%10.4f] FPS:[%d] %10.4f",
		m_fAccumulation, m_iFPS, m_fSecondPerFrame);
	//OutputDebugString(m_csBuffer);
	return true; 
};
// 클래스 멤버 변수 소멸을 담당한다.
bool		TTimer::Release() 
{ 
	return true; 
};

TTimer::TTimer()
{
	m_iFPS	= 0;// 1초당 프레임 개수
	m_dwFrameCnt = 0;
	m_fSecondPerFrame	= 0.0f; // 1프레임의 경과시간
	m_fAccumulation		= 0.0f;// 실행된 경과 시간
	m_fFrameTime		= 0.0f;
}


TTimer::~TTimer()
{
}
