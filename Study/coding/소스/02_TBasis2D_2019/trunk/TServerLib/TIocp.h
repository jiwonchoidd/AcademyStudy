#pragma once
#include "TUserManager.h"

#define MAX_WORKER_THREAD 3

class TIocp : public TSingleton<TIocp>
{
private:
	friend class TSingleton<TIocp>;
public:
	HANDLE	m_hIOCP;
	HANDLE  m_hWorkThread[MAX_WORKER_THREAD];
public:
	bool Init();
	bool Run();
	void SetSocketBind(SOCKET sock, ULONG_PTR key);
public:
	TIocp();
	virtual ~TIocp();
};
#define I_IOCP TIocp::GetInstance()
