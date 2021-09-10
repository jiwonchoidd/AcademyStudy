#pragma once
#include "TCoreStd.h"

class TCOREDLL_API TTimer : public TSingleton<TTimer>
{
public:		
//	friend  class TSingleton<TTimer>;
	TCHAR		m_strBuffer[MAX_PATH];
private:
	// 1초당 프레임 횟수
	int			m_iFPS; 
	// 1프레임이 경과된 시간
	float		m_fSecPerFrame; 
	// 실행파일이 경과된 시간
	float		m_fAccumulation; 
	// 임시변수
	DWORD		m_dwBeforeTick;
	float		m_fFrameTime;
	DWORD		m_dwFrameCnt;
public:	
	bool		Init();	
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	TTimer(void);
	~TTimer(void);
};
#define I_Timer TTimer::GetInstance()
