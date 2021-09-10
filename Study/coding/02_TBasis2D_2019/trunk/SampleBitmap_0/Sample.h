#pragma once
#include "TCore.h"
#include "xBitmap.h"


class Sample : public TCore
{
public:
	xBitmap		m_Bitmap;
	tPOINT		m_ObjPos;
	RECT		m_rtObj;
	HBRUSH		m_hbrBlack;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		PreRender();
public:
	Sample();
	virtual ~Sample();
};
//TCORE_RUN(SampleLib_0, 1024, 768);
TCORE_START;
// �����츦 �����Ѵ�.
if (sample.SetWindow(hInstance,
	L"SampleBitmap_0", 800, 600))
{
	// ������ �����츦 �����Ѵ�.
	sample.Run();
}
TCORE_END;
