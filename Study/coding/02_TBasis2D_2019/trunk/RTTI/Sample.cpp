#include <iostream>
#include <vector>
#define STRING(s) #s // '#'스트링화 연산자
#define ATTACHSTRING(i,j) i##j // '##'  연결연산자

class CBase {};
class CChild : public CBase {};
/////////////////////////////////////////////////////////
#define DECLARE_DYNAMIC(s) static CRuntimeClass class##s;
#define IMPLEMENT_DYNAMIC(s) CRuntimeClass s::class##s={ s::CreateObject };
#define RUNTIME_CLASS(s) &s::class##s
class CObject;
struct CRuntimeClass
{
	CObject* (*pfnCreateObject)();
	CObject* CreateObject()
	{
		return (*pfnCreateObject)();
	}
};
class CObject
{
public:
	std::vector<CObject*> m_list;
	void Add(CRuntimeClass* pRTC2)
	{
		CObject* pClass = pRTC2->CreateObject();
		m_list.push_back(pClass);
	}
};

class CMyClass : public CObject
{
public:
	int m_iIndex = 999;
public:
	DECLARE_DYNAMIC(CMyClass);
	static CObject* CreateObject()
	{
		return new CMyClass;
	}
};
IMPLEMENT_DYNAMIC(CMyClass);
void main()
{
#pragma region TOKEN_STRING
	std::cout << STRING(kgca) << std::endl;
	char g_szData[] = "kgcaAcademy";
	char g_szGame[] = "kgcaAcademyGame";

	std::cout << ATTACHSTRING(g_sz, Data) << std::endl;
	std::cout << ATTACHSTRING(g_sz, Game) << std::endl;
	std::cout << g_szData << std::endl;
	std::cout << g_szGame << std::endl;
	
	//int  g_iData = 0;
	int  ATTACHSTRING(g_i, Data) = 0;
	std::cout << g_iData << std::endl;

	for (int i = 0; i < 3; i++)
	{
		//int iData = i;
		int  ATTACHSTRING(i, Data) = i;
		g_iData += i;
	}
	std::cout << g_iData << std::endl;
#pragma endregion

	CBase parent;
	CChild child;
	const type_info& tBase = typeid(parent);
	const type_info& tChild = typeid(child);
	

	std::cout << tBase.name() << std::endl;
	std::cout << typeid(parent).name() << std::endl;

	CChild* pCastBase = (CChild*)&parent;
	const type_info& pCastChild = typeid(*pCastBase);
	std::cout << pCastChild.name() << std::endl;

	std::cout << "Class CBase" << std::endl;
	std::cout << ATTACHSTRING(typeid(parent), .name()) << std::endl;

	std::cout << tBase.before(tChild) << std::endl; // 1
	std::cout << tChild.before(tBase) << std::endl; // 0
	std::cout << (tBase == tChild) << std::endl;    // 0
	getchar();


	////////////////////////////////////////////////
	CObject mgr;
	mgr.Add(RUNTIME_CLASS(CMyClass));
	CMyClass* pClass = (CMyClass*)mgr.m_list[0];
	std::cout << pClass->m_iIndex << std::endl;
}