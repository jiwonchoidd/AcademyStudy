#include "TTimer.h"
namespace TBASIS_CORE_LIB
{
	float	 g_fGameTimer = 0.0f;
	float    g_fSecondPerFrame = 0.0f;
}

TTimer::TTimer()
{

}
TTimer::~TTimer()
{
	std::cout << "deconstructor";
}

bool  TTimer::Init()
{
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	m_fBeforeTime = timeGetTime();
	return true;
}
bool  TTimer::Reset()
{
	m_fSecondPerFrame = 0.0f;
	m_iFPS = 0;
	m_fBeforeTime = timeGetTime();
	return true;
}
bool  TTimer::Frame() 
{
	// 1/1000 단위로 반환된다.
	float fCurrentTime = timeGetTime();
	m_fSecondPerFrame = 
		(fCurrentTime - m_fBeforeTime) / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	m_fBeforeTime = fCurrentTime;

	g_fSecondPerFrame = m_fSecondPerFrame;
	g_fGameTimer = m_fGameTimer;
	return true;
}
bool  TTimer::Render() 
{
	static float fTimer = 0.0f;
	fTimer += m_fSecondPerFrame;
	if (fTimer >= 1.0f)
	{
		ZeroMemory(m_szBuffer, sizeof(WCHAR) * 256);
		_stprintf_s(m_szBuffer, 
					L"게임시간=%10.2f, spf(%10.4f) fps(%d)\n",
					m_fGameTimer,
					m_fSecondPerFrame,
					m_iFPS);
		//log(1, m_szBuffer);
		fTimer -= 1.0f;
		m_iFPS = 0;
	}

	m_iFPS++;
	return true;
}
bool  TTimer::Release() 
{
	return true;
}