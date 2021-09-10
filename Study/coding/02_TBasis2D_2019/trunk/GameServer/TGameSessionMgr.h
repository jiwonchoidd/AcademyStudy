#pragma once
#include "TSessionMgr.h"
class TGameSessionMgr :public TSingleton<TGameSessionMgr>, public TSessionMgr
{
private:
	friend class TSingleton<TGameSessionMgr>;
public:
	TGameSessionMgr();
	~TGameSessionMgr();
};
#define I_Session TGameSessionMgr::GetInstance()