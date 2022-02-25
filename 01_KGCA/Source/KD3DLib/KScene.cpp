#include "KScene.h"

bool KScene::Init(ID3D11DeviceContext* context)
{
	m_pContext = context;
	return true;
}

bool KScene::Frame()
{
	for (auto obj : m_UIObj)
	{
		KObject* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	for (auto obj : m_MapObj)
	{
		KObject* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	return true;
}

bool KScene::Render()
{
	for (auto obj : m_MapObj)
	{
		KObject* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Render(m_pContext);
		}
	}
	for (auto obj : m_UIObj)
	{
		KObject* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Render(m_pContext);
		}
	}
	return true;
}

bool KScene::Release()
{
	for (auto obj : m_UIObj)
	{
		if(obj.second)
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_MapObj)
	{
		if (obj.second)
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_ItemObj)
	{
		if (obj.second)

		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_CharaterObj)
	{
		if (obj.second)
		obj.second->Release();
		delete obj.second;
	}
	m_UIObj.clear();
	m_MapObj.clear();
	m_ItemObj.clear();
	m_CharaterObj.clear();
	return true;
}

KScene::KScene()
{
	m_pContext = nullptr;
	m_SceneID = -1;
}

KScene::~KScene()
{
}
