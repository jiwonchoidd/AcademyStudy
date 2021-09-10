#pragma once
#include "TCoreStd.h"
class TTimer: public TSingleton<TTimer>
{
private:
	friend class TSingleton<TTimer>;
public:
	// 64∫Ò∆Æ ( 1844∞Ê )
	LARGE_INTEGER  m_Start;
	LARGE_INTEGER  m_End;
	LARGE_INTEGER  m_Frequency;
public:
	DWORD   m_dwStartTime;
	DWORD   m_dwBeforeTime;
	DWORD	m_dwFPS;
	DWORD	m_dwSecPerFrame;
	TCHAR   m_strBuffer[MAX_PATH];
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();	
public:
	TTimer(void);
	~TTimer(void);
};
#define I_Timer TTimer::GetInstance()
