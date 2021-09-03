#pragma once
#include "TServerObj.h"
#include <process.h>
class TThread : public TServerObj
{
public:
	unsigned int m_hThread;
	unsigned int m_iID;
	bool		 m_bStarted;
public:
	void CreateThread();
	virtual bool Run();
	static unsigned int WINAPI HandleRunner(LPVOID prameter);
public:
	TThread();
	virtual ~TThread();
};

