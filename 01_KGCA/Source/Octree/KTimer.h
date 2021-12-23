#pragma once
#include <windows.h>
class KTimer
{
private:
	LARGE_INTEGER m_Frequency; // ÁÖÆÄ¼ö
	LARGE_INTEGER m_Current;
	LARGE_INTEGER m_Frame;
public :
	float	m_fFramePerSec;
	float	m_fGameTimer;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	KTimer();
	~KTimer();
	
};
extern float g_fTimer;
extern float g_fSecPerFrame;

