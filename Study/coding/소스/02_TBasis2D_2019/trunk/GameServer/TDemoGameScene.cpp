#include "TDemoGameScene.h"
#include "TObjectMgr.h"
TObject*  TDemoGameScene::NewObj(TObjAttribute& info)
{
	TObject* pObj = TScene::NewObj(info);
	if (pObj == nullptr)
	{
		if (info.iObjType == TDemoEnum::T_AIRNPC)
		{
			pObj = new TNetObject;
			TNetObject* pNpc = dynamic_cast<TNetObject*>(pObj);
			if (pNpc != nullptr)
			{
				pNpc->FSM();
			}
		}		
	}
	return pObj;
}
void TDemoGameScene::AddNpc(wstring name, TPoint pos, TPoint dir, int iIndex)
{
	TObjAttribute info = m_ObjAttribute[0];
	TObject* pObj = NewObj(info);
	pObj->Init();
	pObj->m_iIndex = iIndex;
	pObj->m_szName = name;
	pObj->m_Attribute = info;
	if (info.bColorKey == false)
	{
		pObj->Load(info.colorbitmap.c_str(),info.maskbitmap.c_str());
	}
	else
	{
		pObj->Load(info.colorbitmap.c_str(),nullptr,info.dwColor);
	}
	pObj->Set(pos, info.rtSrc);
	//pObj->SetSpeed(70.0f);
	m_NpcList.insert(make_pair(iIndex,pObj));
	m_iMaxCounter = (int)m_NpcList.size();
}
bool  TDemoGameScene::Load(const TCHAR* filename)
{
	Init();
	TScene::m_pCurrentScene = this;
	g_ObjectMgr.Load(L"../../data/script/rtSprite.txt");
	if (g_ObjectMgr.LoadObjectFile(filename, m_ObjAttribute))
	{
		return CreateScene();
	}
	return false;
}
bool  TDemoGameScene::Frame()
{
	TScene::Frame();
	return true;
}

bool  TDemoGameScene::Render()
{
	TScene::Render();
	return true;
}
bool  TDemoGameScene::Release()
{
	TScene::Release();
	return true;
}