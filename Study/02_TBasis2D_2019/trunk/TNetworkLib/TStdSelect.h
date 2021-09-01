#pragma once
#include "TSelectModel.h"
class TStdSelect : public TSelectModel
{
public:
	FD_SET  rSet;
	FD_SET  wSet;
public:
	virtual bool	Init();
	virtual bool	Frame();
public:
	TStdSelect(SOCKET sock);
	virtual ~TStdSelect();
};

