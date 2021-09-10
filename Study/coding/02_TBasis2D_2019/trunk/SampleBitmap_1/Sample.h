#pragma once
#include "TCore.h"
#include "xObject.h"

const int g_iMaxNPC = 20;
class Sample : public TCore
{
public:
	xObject		m_Ground;
	xObject		m_Hero;
	xObject		m_npc[g_iMaxNPC];
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	Sample();
	virtual ~Sample();
};
//TCORE_RUN(SampleLib_0, 1024, 768);
TCORE_START;
// 윈도우를 생성한다.
if (sample.SetWindow(hInstance,
	L"SampleBitmap_0", 800, 600 ))//, WS_POPUPWINDOW))
{
	// 생성된 윈도우를 가동한다.
	sample.Run();
}
TCORE_END;
