#pragma once
#include "TWindow.h"
#include "TTimer.h"
#include "TInput.h"
class TCore : public TWindow
{
public:
	TTimer		m_Timer;
	TInput		m_Input;
	HDC			m_hScreenDC;   // ���� ����
	HBITMAP		m_hOldBitmap; // �ĸ������ �̹���

	HDC			m_hOffScreenDC;// �ĸ� ����
	HBITMAP		m_hOffScreenBitmap; // �ĸ������ �̹���	
public:
	bool	GameRun();
	bool	GameInit();	
	bool	GameFrame();
	bool	GameRender();
	bool    GameRelease();
	void	MsgEvent(MSG msg);

	bool	DrawDebug();
	bool	PreRender();
	bool	PostRender();
public:
	////////// ���� �Լ��� ///////////
	virtual bool		Init() { return true; }
	virtual bool		Frame() { return true; }
	virtual bool		Render() { return true; }
	virtual bool		Release() { return true; };
public:
	TCore();
	virtual ~TCore();
};

