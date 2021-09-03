#include <iostream>
#include <typeinfo.h>
#define DECLARE_CLASSNAME static char lpszClassName[]
#define IMPLEMENT_CLASSNAME(s) char s##::lpszClassName[] = (#s);

// 1)��ũ���Լ� ����
class CObject
{
public:
	virtual char* GetClassName() const
	{
		return NULL;
	}
};
class CMyClass : public CObject
{
public:
	DECLARE_CLASSNAME;
	virtual char* GetClassName() const
	{
		return lpszClassName;
	}
};
IMPLEMENT_CLASSNAME(CMyClass);

void main()
{
	CObject* p = new CMyClass;
	std::cout << p->GetClassName() << std::endl;
	delete p;
}
// 1)�Ϲ��ڵ� ����
//class CObject
//{
//public:
//	virtual char* GetClassName() const
//	{
//		return NULL;
//	}
//};
//class CMyClass : public CObject
//{
//public:
//	static char lpszClassName[];
//	virtual char* GetClassName() const
//	{
//		return lpszClassName;
//	}
//};
//char CMyClass::lpszClassName[] = "CMyClass";
//void main()
//{
//	CObject* p = new CMyClass;
//	std::cout << p->GetClassName() << std::endl;
//	delete p;
//}