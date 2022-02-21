#pragma once
#include "KWindow.h"
#include "KDevice.h"
#include "KTimer.h"
#include "KInput.h"
#include "KSoundManager.h"
#include "KWrite.h"
#include "KState.h"
#include "ImGuiManager.h"
class KCore : public KWindow
{
public:
	KTimer			m_Timer;
	KSoundManager	m_Sound;
	KWrite			m_Write;
	ImGuiManager	m_ImGuiManager;
public:
	float		m_Speed=1.0f;
	bool		m_bDebugText = false;
	bool		m_bFreeCamera = false;
public:
	virtual bool	CreateResizeDevice(UINT iWidth, UINT iHeight);
	virtual bool	DeleteResizeDevice(UINT iWidth, UINT iHeight);
	bool		ResizeDevice(UINT iWidth, UINT iHeight);
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
	KCore();
	virtual ~KCore();
};

