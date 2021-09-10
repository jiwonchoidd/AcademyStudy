#pragma once
#include "TCore.h"
#include "TAcceptor.h"

class Sample: public TCore
{
public:
	TAcceptor			m_Acceptor;

public:
	bool				Init();
	bool				PreRender();
	bool				Render();
	bool				PostRender();
	bool				Release();	
public:
	Sample(void);
	~Sample(void);
};
