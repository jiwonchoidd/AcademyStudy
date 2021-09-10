#pragma once
#include "TBasisStd.h"
#include "TServerobj.h"

class TSynchronize 
{
private:
	TServerObj*		m_pServerObj;
public:
	TSynchronize( TServerObj* pSvrObj );
	virtual ~TSynchronize(void);
};
