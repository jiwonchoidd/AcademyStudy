#pragma once
#include "KObject.h"

//�浹 ������ � �Լ� ȣ������ ���� �ý���
using CollisionFunction = std::function<void(KObject*, DWORD)>;

//���콺 ������ ��ư ������ ó���ϴ� �ý���
using SelectFunction = std::function<void(KObject*, DWORD)>;

class KObjectManager : public Singleton <KObjectManager>
{
	std::map<std::wstring, KObject> map;
public:
	friend class Singleton<KObjectManager>;
public:
	//������ ���ǿ� �ش��� �� �ְԲ�
	//
	using CallFunction = std::function<void>;

public:
	//�浹�� �߻��ϸ� � �Լ��� ȣ���ұ� ����
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

