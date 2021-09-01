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
// 윈도우를 생성한다.
if (sample.SetWindow(hInstance, L"SampleRotation_0", 800, 600 ))//, WS_POPUPWINDOW))
{
	// 생성된 윈도우를 가동한다.
	sample.Run();
}
TCORE_END;
