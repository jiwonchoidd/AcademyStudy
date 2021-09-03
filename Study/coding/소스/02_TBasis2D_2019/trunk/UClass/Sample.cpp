#include <iostream>

class UClass;
class UObject
{
public:
	int m_iData;
public:
	static UClass* ClassPrivate;
	static void SetClass(UClass* newClass) { ClassPrivate = newClass; }
	static UClass* GetClass() { return ClassPrivate; }
public:
	UObject() {}
	UObject(int iValue) : m_iData(iValue) {}
	UObject(const UObject& source) {
		m_iData = source.m_iData;
		SetClass(source.GetClass());
	}
};
class CDO :public UObject { };
class UClass
{
	UObject* cdo;
public:
	UObject* GetDefalutObject() { return cdo; }
	void  SetDefalultObject(UObject* pObj) { cdo = pObj; }
	UClass() { cdo = 0; }
	~UClass() { delete cdo; }
};

class AActor : public UObject
{
public:
	static UClass* ClassPrivate;
	static void SetClass(UClass* newClass) { ClassPrivate = newClass; }
	static UClass* GetClass() { return ClassPrivate; }
public:
	AActor(int iValue) : UObject(iValue) {
	}
	AActor(const AActor& source)
	{
		m_iData = source.m_iData;
		SetClass(source.GetClass());
	}
};

UClass* UObject::ClassPrivate = 0;
UClass* AActor::ClassPrivate = 0;

void main()
{
	// 컴파일 초기화 단계
	UClass* uclass = new UClass;
	AActor* actor = new AActor(5);
	uclass->SetDefalultObject(actor);
	actor->SetClass(uclass);

	// 런타임 단계
	UClass* uclass0 = actor->GetClass();
	UClass* uclass1 = AActor::GetClass();

	AActor bb = *((AActor*)uclass0->GetDefalutObject());
	AActor cc = *((AActor*)uclass1->GetDefalutObject());
	UClass* uclass2 = bb.GetClass();
	UClass* uclass3 = cc.GetClass();

	std::cout << std::hex << uclass0 << std::endl;
	std::cout << std::hex << uclass1 << std::endl;
	std::cout << std::hex << uclass2 << std::endl;

	// 소멸 단계
	delete uclass;
}