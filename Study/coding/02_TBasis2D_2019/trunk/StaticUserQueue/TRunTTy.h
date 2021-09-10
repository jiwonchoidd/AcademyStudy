#pragma once
#include "TServerStd.h"

#define DECLARE_CLASSNAME		static char lpszClassName[]
#define IMPLEMENT_CLASSNAME(s)	char s##::lpszClassName[] = (#s);

#define DECLARE_DYNCREATE		static CObject* CreateObject();
#define IMPLEMENT_DYNCREATE(s)	CObject* s::CreateObject()	{return new s;}

#define DECLARE_DYNAMIC(s)		static CRuntimeClass class##s;
#define IMPLEMENT_DYNAMIC(s)	CRuntimeClass s::class##s={ #s, sizeof(s), s::CreateObject }

#define DECLARE_DYNCLASS_CREATE CRuntimeClass* GetRuntimeClass() const;
#define IMPLEMENT_DYNCLASS_CREATE(s) CRuntimeClass* s::GetRuntimeClass() const{return &class##s;}

#define RUNTIME_CLASS(s) &s::class##s
#define RUNTIME_DECLARE(s)		DECLARE_DYNAMIC(s); DECLARE_DYNCREATE; DECLARE_DYNCLASS_CREATE;
#define RUNTIME_IMPLEMENT(s)	IMPLEMENT_DYNCREATE(s);	IMPLEMENT_DYNAMIC(s); IMPLEMENT_DYNCLASS_CREATE(s);

class CObject;
struct CRuntimeClass
{
	char m_lpszClassName[21];
	int  m_iObjectSize;
	CObject* (*pfnCreateObject)();
	CObject* CreateObject()
	{
		return (*pfnCreateObject)();
	}
};
class CObject
{
public:
	DECLARE_DYNAMIC(CObject);
	DECLARE_CLASSNAME;
	virtual CRuntimeClass* GetRuntimeClass() const
	{
		return NULL;
	}
	virtual ~CObject() {}
protected:
	CObject() {}
};


