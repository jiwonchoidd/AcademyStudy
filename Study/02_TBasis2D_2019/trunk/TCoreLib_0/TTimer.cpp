#include "TTimer.h"
float	g_fSecPerFrame = 0.0f;
bool	TTimer::Init() 
{ 
	m_dwStartTime = timeGetTime();
	m_dwBeforeTime = m_dwStartTime;
	// 기준 주파수
	QueryPerformanceFrequency(&m_Frequency);
	memset(&m_End, 0, sizeof(LARGE_INTEGER));
	memset(&m_Start, 0, sizeof(LARGE_INTEGER));
	//현재시간 =  현재 주파수 / 기준 주파수;
	QueryPerformanceCounter(&m_Start);
	return true; 
};
bool	TTimer::Frame()
{ 
	QueryPerformanceCounter(&m_End);
	g_fSecPerFrame = (m_End.QuadPart-m_Start.QuadPart)
						/ 
					(float)(m_Frequency.QuadPart);
	//if( static float fTime += g_fSecPerFrame > 1.0f )

	// 1, 1프레임의 경과시간
	// 2, 1초에 돌아간 프레임 개수
	//DWORD dwTime = GetTimeCount();

	DWORD dwTime = timeGetTime();

	// 시간의 동기화
	m_dwSecPerFrame = dwTime - m_dwBeforeTime;
	static DWORD dwCnt=0;

	//g_fSecPerFrame = m_dwSecPerFrame / 1000.0f;
	if( (dwTime - m_dwStartTime)  > 1000 )
	{
		m_dwStartTime = dwTime;
		m_dwFPS = dwCnt;
		dwCnt = 0;
	};	
	m_dwBeforeTime = dwTime;	
	dwCnt++;


	m_Start = m_End;
	return true; 
};
bool	TTimer::Render()
{ 
	memset( m_strBuffer, 0, sizeof(TCHAR)*MAX_PATH);
	_stprintf(m_strBuffer, _T("FPS:%d SPF:%d"),
		  m_dwFPS, m_dwSecPerFrame );
	return true; 
};
bool	TTimer::Release()
{ 
	return true; 
};
TTimer::TTimer(void)
{
	m_dwFPS			= 0;
	m_dwSecPerFrame	= 0;
}


TTimer::~TTimer(void)
{
}
