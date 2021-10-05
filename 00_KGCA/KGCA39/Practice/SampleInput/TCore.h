#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTimer.h"
#include "Tinput.h"
class TCore : public TWindow
{
public:
	TTimer		m_Timer;
private:
	bool	GameInit()	override;
	bool	GameRun()	override;
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease()override;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual bool	Release();
public:
};

