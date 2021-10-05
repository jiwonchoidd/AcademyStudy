#pragma once

#include "TStd.h"
class TTimer
{
private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_Current;
	LARGE_INTEGER	m_Frame;
	int				m_iTmpCounter;
public:
	int m_iFPS;
	float m_fSecPerFrame;
	float m_fGameTimer;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	TTimer();
	virtual ~TTimer();
}; 

