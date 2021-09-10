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
// �����츦 �����Ѵ�.
if (sample.SetWindow(hInstance,
	L"SampleBitmap_0", 800, 600 ))//, WS_POPUPWINDOW))
{
	// ������ �����츦 �����Ѵ�.
	sample.Run();
}
TCORE_END;
