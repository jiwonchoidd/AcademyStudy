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
	// 윈도우를 생성한다.
	if (sample.SetWindow(hInstance,
		L"SampleWin", 800, 600, WS_POPUPWINDOW))
	{
		// 생성된 윈도우를 가동한다.
		sample.Run();
	}
TCORE_END;
