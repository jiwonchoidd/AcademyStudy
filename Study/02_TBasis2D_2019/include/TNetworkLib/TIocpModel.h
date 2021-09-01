#pragma once
#include "TSelectModel.h"
#define MAX_WORKER_THREAD 3

class TIocpModel : public TSelectModel
{
public:
	HANDLE		m_hIocp;
	HANDLE		m_hWorkThread[MAX_WORKER_THREAD];
public:
	virtual bool	Init() override;
	virtual bool	Frame()override;
	virtual bool	Release()override;
public:
	void SetSocketBind(SOCKET sock, ULONG_PTR key);
public:
	TIocpModel(SOCKET sock);
	virtual ~TIocpModel();
};

