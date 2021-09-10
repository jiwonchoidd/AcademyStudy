#pragma once
#include "TThread.h"
class TWorkThread : public TThread
{
public:
	HANDLE		m_hIOCP;
	bool		Run();
	void		Set( HANDLE hIOCP ){
		m_hIOCP = hIOCP;
	};
public:
	TWorkThread(void);
	virtual ~TWorkThread(void);
};

