#include "TScene.h"
#include "TObjectMgr.h"
int	TScene::m_iStageCounter = 1;
TScene*	TScene::m_pCurrentScene = nullptr;

TGameUser*	TScene::m_pGamePlayer = nullptr;
LRESULT	 TScene::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	for (auto iter : m_UIObjList)
	{
		TUserInterface* pUI = (TUserInterface*)iter.second;
		int iRet = pUI->MsgProc(hWnd, msg, wParam, lParam);
		if (iRet >= 0) return iRet;
	}
	return -1;
};
TObject*  TScene::NewObj(TObjAttribute& info)
{
	TObject* pObject = nullptr;
	switch (info.iObjType)
	{
		case TEnum::T_MAP: 
		{
			pObject = new TObject; 
		}break;
		case TEnum::T_USER:
		{
			pObject = new TGameUser; 
		}break;
		case TEnum::T_NPC: 
		{
			pObject = new TNpcObject; 
			TNpcObject* pNpc = dynamic_cast<TNpcObject*>(pObject);
			if (pNpc != nullptr)
			{
				pNpc->FSM();
			}
		} break;
		case TEnum::T_INTERFACE: {pObject = new TUserInterface; } break;
		case TEnum::T_DIALOGBOX: 
		{
			pObject = new TDialogBox; 
		} break;
		case TEnum::T_BUTTON: {pObject = new TButton; } break;
		case TEnum::T_EDIT: {pObject = new TEdit; } break;
	}

	return pObject;
}
bool  TScene::CreateScene()
{
	m_bSceneChange = false;
	for (TObjAttribute& info : m_ObjAttribute)
	{
		TObject* pObj = NewObj(info);
		if (pObj == nullptr) continue;

		pObj->m_iObjectType = info.iObjType;
		pObj->m_Attribute = info;
		pObj->m_szName = info.szName;
		pObj->m_szParentName = info.szParentName;
		pObj->Init();				
		
		if (info.bColorKey == false)
		{
			pObj->Load(info.colorbitmap.c_str(),info.maskbitmap.c_str());
		}
		else
		{
			pObj->Load(info.colorbitmap.c_str(),nullptr,info.dwColor);
		}
		// state
		pObj->m_StateBitmap.resize(4);
		pObj->m_StateBitmap[DEFAULT] =	pObj->m_pColorBmp;
		if (!info.pushbitmap.empty())
		{
			pObj->m_StateBitmap[PUSH] =	g_BitmapMgr.Load(info.pushbitmap.c_str());
		}
		if (!info.selectbitmap.empty())
		{
			pObj->m_StateBitmap[SELECT] =g_BitmapMgr.Load(info.selectbitmap.c_str());
		}
		if (!info.disbitmap.empty())
		{
			pObj->m_StateBitmap[DISABLE] =	g_BitmapMgr.Load(info.disbitmap.c_str());
		}
		
		SetObject(info, pObj);	
		if (info.iObjType == TEnum::T_DIALOGBOX)
		{
			pObj->m_iSortValue += 100;
		}
		if (info.iObjType == TEnum::T_EDIT)
		{
			TEdit* pEdit = (TEdit*)pObj;
			pEdit->m_rtWndCtrl = pObj->m_rtDesk;
			if (pObj->m_pParentObject != nullptr)
			{
				pEdit->m_rtWndCtrl.left = pObj->m_pParentObject->m_rtCollide.left + pObj->m_rtDesk.left;
				pEdit->m_rtWndCtrl.top = pObj->m_pParentObject->m_rtCollide.top + pObj->m_rtDesk.top;
			}
			pEdit->CreateEdit();
		}
	}

	SortZValue(m_UIObjList, m_UIDrawObjList);
	return true;
}
void  TScene::SortZValue(std::map<wstring, TObject*>& srcList, std::vector<std::pair<wstring, TObject*>>& destList)
{
	auto cmp = [](std::pair<wstring, TObject*>  a, std::pair<wstring, TObject*>  b)
	{
		TObject* pA = a.second;
		TObject* pB = b.second;
		return pA->m_iSortValue < pB->m_iSortValue;
	};
	std::copy(srcList.begin(), srcList.end(), std::back_inserter(destList));
	std::sort(destList.begin(), destList.end(), cmp);
}
bool  TScene::SetObject(TObjAttribute& info, TObject* pObj)
{
	assert(pObj);

	if (pObj && !(info.szParentName == L"nullptr" || info.szParentName == L"NULL"))
	{
		pObj->m_pParentObject = FindParent(info.szParentName);
		if (pObj->m_pParentObject != nullptr)
		{
			pObj->m_pParentObject->m_pChildObjects.push_back(pObj);
		}
	}

	switch (info.iObjType)
	{
	case TEnum::T_USER:
	{
		m_pGamePlayer = (TGameUser*)pObj;
		m_ptInitHeroPos = info.pos;
		return true;
	}break;
	case TEnum::T_NPC:
	{
		pObj->Set(info.pos, info.rtSrc);
		if (pObj->m_pParentObject == nullptr)
		{
			m_CharacterList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case TEnum::T_OBJECT:
	{
		pObj->Set(info.pos, info.rtSrc);
		if (pObj->m_pParentObject == nullptr)
		{
			m_ObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case TEnum::T_EDIT:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		if (pObj->m_pParentObject == nullptr)
		{
			m_UIObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case TEnum::T_INTERFACE:
	case TEnum::T_BUTTON:
	case TEnum::T_DIALOGBOX:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		pObj->Update();
		if (pObj->m_pParentObject == nullptr)
		{
			m_UIObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case TEnum::T_MAP:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		if (pObj->m_pParentObject == nullptr)
		{
			m_MapList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	}
	return false;
}
bool  TScene::Load(const TCHAR* filename)
{
	Init();	
	if (!g_ObjectMgr.LoadObjectFile(filename, m_ObjAttribute))
	{
		return false;
	}
	return true;
}
bool  TScene::Init()
{
	for (auto iter : m_UIObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Init();
	}
	for (auto iter : m_ObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj)	pObj->Init();
	}
	return true;
}
bool  TScene::PreFrame()
{
	return true;
}
bool  TScene::Frame()
{
	for (auto iter : m_MapList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}	
	for (auto iter : m_ObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	for (auto iter : m_NpcList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	for (auto iter : m_UserList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	//effect frame
	for (std::vector<TEffectInfo>::iterator iter = m_EffectList.begin();
		iter != m_EffectList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_EffectList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (auto iter : m_UIDrawObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	TButton* pExitBtn = (TButton*)FindUI(L"GAME_EXIT");
	if (pExitBtn &&  pExitBtn->m_bSelect == true)
	{
		TScene::m_pCurrentScene->m_bGameFinish = true;
	}
	return true;
}
bool  TScene::PostFrame()
{
	return true;
}
bool  TScene::Release()
{
	for (auto iter : m_MapList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ItemObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_UIObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_UserList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_NpcList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	delete m_pGamePlayer;
	m_pGamePlayer = nullptr;
	m_MapList.clear();
	m_ObjList.clear();
	m_ItemObjList.clear();
	m_UIObjList.clear();
	m_ObjAttribute.clear();
	m_UserList.clear();
	m_NpcList.clear();
	m_EffectList.clear();
	return true;
}
bool  TScene::PreRender()
{
	return true;
}
bool  TScene::Render()
{	
	PreRender();
	{
		RenderMap();
		RenderObject();
		RenderCharacter();
		RenderEffect();
		RenderUI();
	}
	PostRender();
	return true;
}
bool  TScene::PostRender()
{
	if (TScene::m_pCurrentScene->m_bGameFinish)
	{
		SendMessage(g_hWnd, WM_DESTROY, 0, 0);
		return false;
	}
	return true;
}
bool  TScene::RenderMap()
{
	for (auto iter : m_MapList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render();
		}
	}
	return true;
}
bool  TScene::RenderUI()
{
	for (auto iter : m_UIDrawObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render();
		}
	}
	return true;
}
bool  TScene::RenderEffect()
{
	for (auto& pInfo : m_EffectList)
	{
		TEffect* pEffect = (TEffect*)g_ObjectMgr.GetPtr(pInfo.m_csName);
		pEffect->Set(pInfo.p,
			pEffect->m_rtList[pInfo.m_iRectIndex]);
		pEffect->Render();
	}
	return true;
}
bool  TScene::RenderObject()
{
	for (auto iter : m_ObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_bDead == false)
		{
			pObj->Render();
		}
	}
	return true;
}
bool  TScene::RenderCharacter()
{
	for (auto iter : m_NpcList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render();
		}
	}
	for (auto iter : m_UserList)
	{
		TObject* pObj = (TObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render();
		}
	}
	return true;
}
void  TScene::AddEffect(wstring name, TPoint pos, TPoint dir)
{
	TEffect* pEffect = (TEffect*)g_ObjectMgr.GetPtr(name);
	if (pEffect == nullptr) return;
	TEffectInfo info;
	info.m_csName = name;
	info.m_bDead = false;
	info.m_fLifeTime = 1.0f;
	info.iMaxCount = (int)pEffect->m_rtList.size();
	info.m_fStep = 1.0f / pEffect->m_rtList.size();
	info.m_fStep = 0.1f;
	info.d = { dir.x, dir.y };
	info.p = { pos.x, pos.y };
	m_EffectList.push_back(info);
}
void  TScene::AddNpc(wstring name, TPoint pos, TPoint dir, int index)
{
};
bool  TScene::Reset()
{
	for (auto iter : m_UIObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ItemObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ObjList)
	{
		TObject* pObj = (TObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	
	m_UIObjList.clear();
	m_ItemObjList.clear();
	m_ObjList.clear();
	if (m_pGamePlayer)
	{
		m_pGamePlayer->Reset();
		m_pGamePlayer->SetPos(m_ptInitHeroPos);
	}
	m_EffectList.clear();

	m_iNpcCounter = 0;
	return true;
}
TObject*	TScene::FindParent(std::wstring szName)
{
	TObject* pParentObj = FindMap(szName);
	if (pParentObj == nullptr)
	{
		pParentObj = FindUI(szName);
		if (pParentObj == nullptr)
		{
			pParentObj = FindItem(szName);
			if (pParentObj == nullptr)
			{
				pParentObj = FindObject(szName);
				if (pParentObj == nullptr)
				{
					return nullptr;
				}
			}
		}
	}
	return pParentObj;
}
TObject*	TScene::FindMap(std::wstring szName)
{
	auto iter = m_MapList.find(szName);
	if (iter == m_MapList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
TObject*	TScene::FindUI(std::wstring szName)
{
	auto iter = m_UIObjList.find(szName);
	if (iter == m_UIObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
TObject*	TScene::FindItem(std::wstring szName)
{
	auto iter = m_ItemObjList.find(szName);
	if (iter == m_ItemObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
};
TObject*	TScene::FindObject(std::wstring szName)
{
	auto iter = m_ObjList.find(szName);
	if (iter == m_ObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
TObject*	TScene::FindNpc(int iIndex)
{
	auto iter = m_NpcList.find(iIndex);
	if (iter == m_NpcList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
bool	TScene::DelNpc(int iIndex)
{
	auto iter = m_NpcList.find(iIndex);
	if (iter != m_NpcList.end())
	{
		g_ObjectMgr.DeleteExecute(iter->second);
		delete iter->second;
		m_NpcList.erase(iter);		
		return true;
	}
	return false;
}
TScene::TScene()
{
	m_iStageCounter = 0;
	m_iSceneID		= 0;
	m_bSceneChange	= false;
	m_iNextScene	= TSCENE_LOADING;
	m_iNpcCounter	= 0;
}
TScene::~TScene()
{

}