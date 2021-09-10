#pragma once
#include "TWindow.h"
#include "TTimer.h"
#include "TInput.h"
#include "TGraphicAPI.h"
#include "TWrite.h"
#include "TSoundManager.h"

class TCore : public TWindow
{
public:
	bool		m_bGameRun;
	TGraphicAPI m_Graphic;	
public:
	virtual bool	Init() {
		return true;
	};
	virtual bool	PreFrame() {
		return true;
	};
	virtual bool	Frame() {
		return true;
	};
	virtual bool	PostFrame() {
		return true;
	};
	virtual bool	Render() {
		return true;
	};
	virtual bool	PreRender();
	virtual bool	PostRender();

	virtual bool	Release() {
		return true;
	};
private:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease();
	bool	GameRun();
public:
	bool	Run();
	TCore();
	virtual ~TCore();
public:
	bool	OpenDebugConsole();
};

