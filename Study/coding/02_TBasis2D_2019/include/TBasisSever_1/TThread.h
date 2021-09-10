#pragma once
#include "TSynchronize.h"
class TThread : public TServerObj
{
public:
	unsigned int   m_hThread;
	unsigned int   m_iThreadID;
	bool		   m_bStarted;
public:
	void	CreateThread();
	virtual bool Run();
	static unsigned int WINAPI HandleRunner(LPVOID parameter);
public:
	TThread(void);
	virtual ~TThread(void);
};

