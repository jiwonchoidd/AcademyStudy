#pragma once
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TWrite.h"
#include "TDxState.h"
#include "TDebugCamera.h"
#include "TShape.h"
class TCoreDx : public TDevice
{
public:
	TTimer				m_Timer;
	TInput				m_Input;
	TWrite				m_Write;
	TCamera*			m_pMainCamera;
	TCamera				m_DirCamera;
	TDebugCamera		m_DebugCamera;
	TLineAxisShape		m_LineAxis;
	D3DXMATRIX			m_matWorldLine;
public:
	bool	GameRun();
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool    GameRelease();
	bool	PreCoreFrame();
	bool	PostCoreFrame() { m_Input.m_sMouseWheelDelta = 0; return true; }
	bool	PreCoreRender();
	bool	PostCoreRender();
	void	MsgEvent(MSG msg);
public:
	////////// 가상 함수들 ///////////
	virtual bool	DrawDebug();
	virtual bool    DrawDebug(TCHAR* pString, int iX, int iY);
	virtual bool	PreRender();
	virtual bool	PostRender();	
	virtual bool	Init() { return true; }
	virtual bool	PreFrame() { return true; };
	virtual bool	PostFrame(){return true; }
	virtual bool	Frame() { return true; }
	virtual bool	Render() { return true; }
	virtual bool	Release() { return true; };
			bool	ResizeClient(UINT iWidth, UINT iHeight);
public:
	TCoreDx();
	virtual ~TCoreDx();
};

