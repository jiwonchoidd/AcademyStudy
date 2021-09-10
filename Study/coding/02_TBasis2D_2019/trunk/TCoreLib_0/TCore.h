#pragma once
#include "TWindow.h"

#include "TObject.h"

class TCore : public TWindow
{
public:
	HDC			m_hScreenDC;
	TObject		m_ObjectBG;
	HDC	        GetOffDC();
public:
	bool		TCoreInit();
	bool		TCoreFrame();
	bool		TCoreRender();
	bool		TCoreRelease();
public:
	virtual bool	Init()  { return true; };
	virtual bool	Frame(){ return true; };
	virtual bool	Render(){ return true; };
	virtual bool	Release(){ return true; };
	virtual bool	Run();
	virtual bool	PreRender();
	virtual bool	PostRender();
public:
	TCore(void);
	~TCore(void);
};

