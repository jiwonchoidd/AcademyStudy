#pragma once
#include "TNetwork.h"
#include "TCore.h"
#include "TImage.h"

class Sample : public TCore
{
public:
	static TNetwork	m_Network;
	TImage			m_ImageBK;
	TImage			m_ImageEdit;
	TImage			m_ImageButton;
public:
	bool		Init();
	bool		PreFrame();
	bool		Frame();
	bool		Render();
	bool		PostRender();
	bool		Release();
	bool		PacketProcess();

	LRESULT MsgProc(HWND hWnd,UINT msg,	WPARAM wParam,LPARAM lParam);

public:
	Sample();
	virtual ~Sample();
};

