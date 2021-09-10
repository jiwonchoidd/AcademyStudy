#pragma once
#include <process.h>
#include "TBasisStd.h"

class TThread 
{
public:
	unsigned int	m_hThread;
	unsigned int	m_iThreadID;
	bool			m_bStarted;
public:
	void		CreateThread();
	static unsigned int  WINAPI HandleRunner( LPVOID parameter );
	virtual bool	Run() 
	{
		return true;
	}
public:
	TThread(void);
	virtual ~TThread(void);
};
