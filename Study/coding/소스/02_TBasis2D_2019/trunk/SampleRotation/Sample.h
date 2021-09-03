#pragma once
#include "TCore.h"
#include "xObject.h"

const int g_iMaxNPC = 30;
class Sample : public TCore
{
public:
	xObject		m_Ground;
	xObject		m_Hero;
	xObject		m_npc[g_iMaxNPC];
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
//TCORE_RUN(SampleLib_0, 1024, 768);
TCORE_START;
// �����츦 �����Ѵ�.
if (sample.SetWindow(hInstance, L"SampleRotation_0", 800, 600 ))//, WS_POPUPWINDOW))
{
	// ������ �����츦 �����Ѵ�.
	sample.Run();
}
TCORE_END;
