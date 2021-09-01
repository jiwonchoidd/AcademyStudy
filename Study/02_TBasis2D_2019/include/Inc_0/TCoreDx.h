#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWrite.h"
#include "TDxState.h"
class TCoreDx : public TDevice
{
public:
	TTimer		m_Timer;
	TInput		m_Input;
	TWrite		m_Write;
public:
	bool	GameRun();
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool    GameRelease();
	void	MsgEvent(MSG msg);
	virtual bool	DrawDebug();
	virtual bool    DrawDebug(TCHAR* pString, int iX, int iY);
	virtual bool	PreRender();
	virtual bool	PostRender();
public:
	////////// 가상 함수들 ///////////
	virtual bool		Init() { return true; }
	virtual bool		PreFrame();
	virtual bool		PostFrame() { m_Input.m_sMouseWheelDelta = 0; return true; }
	virtual bool		Frame() { return true; }
	virtual bool		Render() { return true; }
	virtual bool		Release() { return true; };
			bool		ResizeClient(UINT iWidth, UINT iHeight);
public:
	TCoreDx();
	virtual ~TCoreDx();
};

