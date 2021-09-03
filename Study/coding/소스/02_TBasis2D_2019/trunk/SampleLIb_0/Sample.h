#pragma once
#include "TCore.h"
#include "TBitmapTmp.h"

class Sample : public TCore
{
public:
	TBitmapTmp		m_Bitmap;
public:
	bool		Init();
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
		L"SampleWin", 800, 600, WS_POPUPWINDOW))
	{
		// ������ �����츦 �����Ѵ�.
		sample.Run();
	}
TCORE_END;
