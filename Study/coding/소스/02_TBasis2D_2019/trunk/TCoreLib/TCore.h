#pragma once
#include "TWindow.h"
#include "TTimer.h"
#include "TInput.h"
#include "TScriptMgr.h"
#include "TWrite.h"
class TCore : public TWindow
{
public:
	TTimer		m_Timer;
	HDC			m_hScreenDC;   // 전면 버퍼
	HBITMAP		m_hOldBitmap; // 후면버퍼의 이미지

	HDC			m_hOffScreenDC;// 후면 버퍼
	HBITMAP		m_hOffScreenBitmap; // 후면버퍼의 이미지	

	HBRUSH		m_hbrBack;
	HBRUSH		m_hbrOld;

	HANDLE		m_hPen;
	HANDLE		m_hOldPen;

	HFONT		m_fontDefault;
	HFONT		m_fontDefaultOld;

public:
	bool	GameRun();
	bool	GameInit();	
	bool	GameFrame();
	virtual bool	GameRender();
	bool    GameRelease();
	void	MsgEvent(MSG msg);
	bool	ResizeClient(UINT iWidth, UINT iHeight);
	virtual bool	DrawDebug();
	virtual bool    DrawDebug(TCHAR* pString, int iX, int iY);
	virtual bool	PreRender();
	virtual bool	PostRender();
public:
	////////// 가상 함수들 ///////////
	virtual bool		Init() { return true; }
	virtual bool		PreFrame() { return true; }	
	virtual bool		PostFrame() { return true; }	
	virtual bool		Frame() { return true; }
	virtual bool		Render() { return true; }
	virtual bool		Release() { return true; };
public:
	TCore();
	virtual ~TCore();
};

