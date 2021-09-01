#include <iostream>
#include <typeinfo.h>
#include <list>
#define DECLARE_CLASSNAME static char lpszClassName[];
#define IMPLEMENT_CLASSNAME(s) char s##::lpszClassName[] = (#s);

#define DECLARE_DYNCREATE static CObject* CreateObject();
#define IMPLEMENT_DYNCREATE(s) CObject* s::CreateObject()	{return new s;};

#define DECLARE_DYNAMIC(s) static CRuntimeClass class##s;
#define IMPLEMENT_DYNAMIC(s) CRuntimeClass s::class##s={ #s, sizeof(s), s::CreateObject };

#define DECLARE_DYNCLASS_CREATE CRuntimeClass* GetRuntimeClass() const;
#define IMPLEMENT_DYNCLASS_CREATE(s) CRuntimeClass* s::GetRuntimeClass() const{return &class##s;};

#define RUNTIME_CLASS(s) &s::class##s;
#define RUNTIME_DECLARE(s) DECLARE_DYNAMIC(s); DECLARE_DYNCREATE; DECLARE_DYNCLASS_CREATE;
#define RUNTIME_IMPLEMENT(s) IMPLEMENT_DYNCREATE(s);	IMPLEMENT_DYNAMIC(s); IMPLEMENT_DYNCLASS_CREATE(s);
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

/*   "////" 주석을 풀면 클래스 정보를 얻게 된다.*/
class CObject
{
public:
	////DECLARE_DYNAMIC(CObject)//static CRuntimeClass classCObject;
	////virtual CRuntimeClass* GetRuntimeClass() const 	{return NULL;}	
	virtual void  Run() { std::cout << "CObject->Run()" << std::endl; }
	virtual ~CObject() {}
protected:
	CObject() {}
};
///CRuntimeClass CObject::classCObject = { "CObject", sizeof(CObject), NULL };

//------------------------------------------------------------------------------------------

class TUser : public CObject
{
public:
	RUNTIME_DECLARE(TUser)
	////DECLARE_DYNAMIC(TUser)		//static CRuntimeClass classCUser;
	////DECLARE_DYNCREATE			// static CObject* CreateObject();
public :
	virtual void  Run() { std::cout << "TUser->Run()" << std::endl; }
	////DECLARE_DYNCLASS_CREATE		//CRuntimeClass* GetRuntimeClass() const;		
protected:
	TUser() {}
};
RUNTIME_IMPLEMENT(TUser)
////IMPLEMENT_DYNCREATE(TUser)			//CObject* TUser::CreateObject() { return new TUser; };
////IMPLEMENT_DYNAMIC(TUser)			//CRuntimeClass TUser::classCUser = { "TUser", sizeof(TUser), TUser::CreateObject };
////IMPLEMENT_DYNCLASS_CREATE(TUser)	//CRuntimeClass* GetRuntimeClass() const { return &classCUser; }

void main()
{
	CRuntimeClass* pRTCMyClass = RUNTIME_CLASS(TUser);
	CObject* p = pRTCMyClass->CreateObject();
	////std::cout << p->GetRuntimeClass()->m_lpszClassName << std::endl;
	p->Run();
	delete p;
}
