#include "KScene.h"

bool KScene::Release()
{
	for (auto obj : m_UIobj)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_Map)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_Item)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_Charater)
	{
		obj.second->Release();
		delete obj.second;
	}
	m_UIobj.clear();
	m_Map.clear();
	m_Item.clear();
	m_Charater.clear();
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
