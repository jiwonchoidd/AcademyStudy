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
// 윈도우를 생성한다.
if (sample.SetWindow(hInstance,
	L"SampleBitmap_0", 800, 600))
{
	// 생성된 윈도우를 가동한다.
	sample.Run();
}
TCORE_END;
