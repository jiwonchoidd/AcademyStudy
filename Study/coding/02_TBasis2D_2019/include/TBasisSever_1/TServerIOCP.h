#pragma once
#include "TWorkThread.h"
#include "TUser.h"
#define WORKDER_THREAD_COUNT 3
class TServerIOCP : public TSingleton<TServerIOCP>, public TServerObj
{
private:
	friend class TSingleton<TServerIOCP>;
public:
	HANDLE			m_hIOCP;
	TWorkThread		m_WorkerThread[WORKDER_THREAD_COUNT];
public:
	bool			Init();	
	bool			Release();
	void			AddHandleToIOCP(
		HANDLE hClientSocket,
		ULONG_PTR dwReturnKey);
public:
	TServerIOCP(void);
	~TServerIOCP(void);
};
#define I_ServerIOCP TServerIOCP::GetInstance()
