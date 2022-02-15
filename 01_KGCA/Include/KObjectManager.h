#pragma once
#include "KObject.h"

//충돌 났을때 어떤 함수 호출할지 결정 시스템
using CollisionFunction = std::function<void(KObject*, DWORD)>;

//마우스 유아이 버튼 누를때 처리하는 시스템
using SelectFunction = std::function<void(KObject*, DWORD)>;

class KObjectManager : public Singleton <KObjectManager>
{
	std::map<std::wstring, KObject> map;
public:
	friend class Singleton<KObjectManager>;
public:
	//세가지 조건에 해당할 수 있게끔
	//
	using CallFunction = std::function<void>;

public:
	//충돌이 발생하면 어떤 함수를 호출할까 결정
	void AddCollisionExecute(KObject* owner, CollisionFunction func);
	void DeleteExecute(KObject* owner, CollisionFunction func);
public:
	bool Init();
	bool Frame();
	bool Release();
private:
	KObjectManager() {};
	~KObjectManager() {};
};
#define g_ObjManager KObjectManager::Get();

