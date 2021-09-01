#pragma once
#include "TWindow.h"
#include "TInput.h"
#include "TTimer.h"

class TCOREDLL_API TCore : public TWindow
{

public:
	//TInput*	m_pInput;	
	//TTimer  m_Timer;
public:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease();
	bool	Run();
	bool	PreRender();
	bool	PostRender();
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	TCore(void);
	virtual ~TCore(void);
};

