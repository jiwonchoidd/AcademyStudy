#pragma once
#include "TSelectModel.h"
class TEventSelect : public TSelectModel
{
public:
	HANDLE		m_EventArray[64];
	bool		Init();
	bool		Frame();
public:
	TEventSelect(SOCKET sock);
	virtual ~TEventSelect();
};

