#pragma once
#include "TCore.h"
#include "TObject.h"

const int g_iMaxNPC = 30;
class Sample : public TCore
{
public:
	TObject		m_Ground;
	TObject		m_Hero;
	TObject		m_npc[g_iMaxNPC];
	bool		m_bnpc[g_iMaxNPC];
	bool		m_bGameEnding;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		RectInPt(RECT rt, POINT pt);
	bool		RectInRect(RECT rtDesk, RECT rtSrc);
	bool		SphereInSphere(RECT rtDesk, RECT rtSrc);
public:
	Sample();
	virtual ~Sample();
};
TCORE_RUN(SampleFlocking, 800, 600);
