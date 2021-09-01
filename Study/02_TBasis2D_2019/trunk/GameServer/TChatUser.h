#pragma once
#include "TNetUser.h"
#include "TNetObject.h"
class TChatUser : public TNetUser
{
public:
	TObject		m_Object;
	TNetUser*   m_pTargetObj;
public:
	TChatUser(TServer* pServer);
	virtual ~TChatUser();
};

