#pragma once
#include "TThread.h"

class TSender : public TThread 
{
public:
	TSender(void);
	virtual ~TSender(void);
};
