#include "TTimer.h"

bool TTimer::Init()
{
	m_iFPS				= 0 ; 
	m_fSecPerFrame		= 0.0f; 
	m_fAccumulation		= 0.0f; 
	m_fFrameTime		= 0.0f;
	m_dwFrameCnt		= 0;
	// 실행파일이 누적된 틱값을 반환한다.
	// 1000 = 1초
	m_dwBeforeTick		= timeGetTime();
	return true;
};
bool TTimer::Frame()
{
	DWORD dwElapseTick = 0;	
	DWORD dwCurrentTick = timeGetTime();
	dwElapseTick = dwCurrentTick - m_dwBeforeTick;
	m_fAccumulation += dwElapseTick / 1000.0f;
	m_fSecPerFrame = dwElapseTick / 1000.0f;
	m_fFrameTime += m_fSecPerFrame;

	if( m_fFrameTime >= 1.0f )
	{
		m_iFPS = m_dwFrameCnt;
		m_dwFrameCnt = 0;
		m_fFrameTime -= 1.0f;
	}
	m_dwFrameCnt++;
	m_dwBeforeTick = dwCurrentTick;
	return true;
};
bool TTimer::Render()
{
	memset( m_strBuffer, 0, sizeof( TCHAR )* MAX_PATH );
	//wsprintf(
	//_stprintf_s
	_stprintf_s( m_strBuffer, 
		_T("FPS:%d, %10.6f %10.6f"), 
		m_iFPS, m_fSecPerFrame, m_fAccumulation);	
	return true;
};
bool TTimer::Release()
{
	return true;
};
TTimer::TTimer(void)
{
}


TTimer::~TTimer(void)
{
}
