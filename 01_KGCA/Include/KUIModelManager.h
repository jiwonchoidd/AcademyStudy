#pragma once
#include "KManager.h"
#include "K2DAsset.h"
class KUIModel : public K2DAsset
{
public:
	KUIModel* m_pParent = nullptr;
public:
	//���� �����ؼ� ��Ȱ��
	//����ؼ� ����ϴ� �Լ�
	virtual KUIModel* Clone() { return nullptr; };
	virtual void  UpdateData() {}
public:
	bool	Load(std::wstring filename) 
	{
		return true;
	};
};

/// <summary>
/// Composite Pattern
/// ��Ȱ���ϴ� �뵵
/// UI�� ������Ÿ���� ����Ʈ�� �ξ������ ������Ʈ�� ����Ʈ�� ����
/// </summary>
class KUIModelComposite: public KUIModel
{
public:
	std::list< KUIModel*> m_Components;
public:
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext* context);
	virtual bool		Release();
	virtual void		Add(KUIModel* pObj);
	virtual KUIModel*	Clone();
	virtual bool		Load(std::wstring filename) 
	{
		return true;
	};
	virtual void		UpdateData();
};
class KUIModelManager : public KManager<KUIModel, KUIModelManager>
{
	friend class Singleton<KUIModelManager>;
private:
	KUIModelManager() {};
public:
	~KUIModelManager() {};
};
#define g_UIModelManager KUIModelManager::Get()

