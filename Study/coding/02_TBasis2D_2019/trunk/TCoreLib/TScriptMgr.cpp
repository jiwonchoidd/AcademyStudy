#include "TScriptMgr.h"
#include "TImageMgr.h"
#include "TInput.h"

bool TScriptMgr::GameDataLoad(T_STR name, TSpriteData& data)
{
	FILE* fp = 0;
	_wfopen_s(&fp, name.c_str(), _T("rt"));
	if (fp == NULL) return false;

	TCHAR buffer[256] = { 0, };
	TCHAR temp[256] = { 0, };
	TCHAR bitmap[256] = { 0, };
	TCHAR mask[256] = { 0, };
	int iNumSprite = -1;

	_fgetts(buffer, _countof(buffer), fp);
	_stscanf_s(buffer, _T("%s%d%s%s"),	temp, (unsigned int)_countof(temp),
		&iNumSprite, bitmap, (unsigned int)_countof(bitmap),mask, (unsigned int)_countof(mask));

	T_STR path = L"../../data/";
	T_STR BitmapPath = path + bitmap;
	T_STR MaskPath = path + mask;
	data.m_iBitmapID = I_ImageMgr.Load(BitmapPath.c_str());
	data.m_iMaskBitmapID = I_ImageMgr.Load(MaskPath.c_str());

	data.m_SpliteListData.resize(iNumSprite);

	int iNumFrame = -1;
	for (int iSprite = 0; iSprite < iNumSprite; iSprite++)
	{
		//TObjectInfo info;
		_fgetts(buffer, _countof(buffer), fp);
		_stscanf_s(buffer, _T("%s%d%f%f%f%f%s%s"),
			temp, (unsigned int)_countof(temp),
			&data.m_SpliteListData[iSprite].iNumFrame,
			&data.m_SpliteListData[iSprite].InitPos.x,
			&data.m_SpliteListData[iSprite].InitPos.y,
			&data.m_SpliteListData[iSprite].fTime,
			&data.m_SpliteListData[iSprite].fLifeTime,
			bitmap, (unsigned int)_countof(bitmap),
			mask, (unsigned int)_countof(mask));

		if (_tcscmp(bitmap, L"null"))
		{
			BitmapPath.clear();
			BitmapPath = path + bitmap;
			data.m_SpliteListData[iSprite].iBitmapID = I_ImageMgr.Load(BitmapPath.c_str());
		}
		if (_tcscmp(mask, L"null"))
		{
			MaskPath.clear();
			MaskPath = path + bitmap;
			data.m_SpliteListData[iSprite].iMaskID = I_ImageMgr.Load(MaskPath.c_str());
		}
		data.m_SpliteListData[iSprite].rtList.resize(
			data.m_SpliteListData[iSprite].iNumFrame);

		int iCount = -1;
		for (int iCnt = 0; iCnt < data.m_SpliteListData[iSprite].iNumFrame; iCnt++)
		{
			_fgetts(buffer, _countof(buffer), fp);
			_stscanf_s(buffer, _T("%d%d%d%d%d"),
				&iCount,
				&data.m_SpliteListData[iSprite].rtList[iCnt].left,
				&data.m_SpliteListData[iSprite].rtList[iCnt].top,
				&data.m_SpliteListData[iSprite].rtList[iCnt].right,
				&data.m_SpliteListData[iSprite].rtList[iCnt].bottom);
		}
	}
	fclose(fp);
	return true;
}
int   TScriptMgr::Load(const TCHAR* pFileName)
{
	TSpriteData data;
	if (GameDataLoad(pFileName, data))
	{
		data.m_iIndex = m_iIndex;
		data.m_szName = pFileName;
		m_list.insert(make_pair(m_iIndex++, data));
	}
	return m_iIndex - 1;
}
TSpriteData*  TScriptMgr::GetPtr(int iKey)
{
	ITOR itor = m_list.find(iKey);
	if (itor == m_list.end())
	{
		return NULL;
	}
	TSpriteData* data = &(*itor).second;
	return data;
}
bool   TScriptMgr::Init() 
{
	m_iNumberSprite = Load(L"../../data/script/Number.txt");
	m_iObjectSprite = Load(L"../../data/script/spriteCoord.txt");
	m_iUISpriete    = Load(L"../../data/script/ui.txt");

	// 내장 스프라이트 객체 생성
	TSpriteData* pData = GetPtr(m_iNumberSprite);
	if (pData != nullptr)
	{
		for (int iSprite = 0; iSprite < pData->m_SpliteListData.size(); iSprite++)
		{
			TSprite s0;
			s0.Init();
			s0.Set(pData->m_SpliteListData[iSprite], pData->m_iBitmapID, pData->m_iMaskBitmapID);
			m_TimerObj.push_back(s0);
		}
	}
	pData = GetPtr(m_iObjectSprite);
	if (pData != nullptr)
	{
		for (int iSprite = 0; iSprite < pData->m_SpliteListData.size(); iSprite++)
		{
			TSprite s0;
			s0.Init();
			s0.Set(pData->m_SpliteListData[iSprite], pData->m_iBitmapID, pData->m_iMaskBitmapID);
			m_EffectObj.push_back(s0);
		}
	}
	pData = GetPtr(m_iUISpriete);
	if (pData != nullptr)
	{
		for (int iSprite = 0; iSprite < pData->m_SpliteListData.size(); iSprite++)
		{
			TSprite s0;
			s0.Init();
			s0.Set(pData->m_SpliteListData[iSprite], pData->m_iBitmapID, pData->m_iMaskBitmapID);
			m_UIObj.push_back(s0);
		}
	}
	return true;
}
bool TScriptMgr::Frame()
{
	//static float fTimer = 0.0f;
	//fTimer += g_fSecPerFrame;
	//if (fTimer >= 1.0f)
	//{
	//	m_iTimerSprite++;
	//	if (m_iTimerSprite > 9)
	//	{
	//		m_iTimerSprite = 0;
	//	}
	//	fTimer = 0.0f;
	//}
	/*for (int iSprite = 0; iSprite < m_EffectObj.size(); iSprite++)
	{
		m_EffectObj[iSprite].Frame();
	}
	for (int iSprite = 0; iSprite < m_UIObj.size(); iSprite++)
	{
		m_UIObj[iSprite].Frame();
	}*/

	if (g_ActionInput.bLeftClick > 0)
	{
		int iRand = rand() % 3;
		TSprite s0 = m_EffectObj[iRand];
		s0.m_info.bLoop = false;
		s0.m_iCurrentFrame = 0;
		s0.m_bDead = false;
		s0.m_info.fLifeTime = rand() % 3;
		s0.SetPos(I_Input.m_MousePos);
		s0.m_info.rtList = m_EffectObj[iRand].m_info.rtList;
		m_SpliteListClick.push_back(s0);
	}
	for (auto& actor : m_SpliteListClick)
	{
		actor.Frame();
	}
	return true;
}
bool TScriptMgr::Render()
{
	//m_UIObj[0].Draw(SRCCOPY);
	//for (int iSprite = 1; iSprite < m_UIObj.size(); iSprite++)
	//{
	//	m_UIObj[iSprite].Draw(MERGEPAINT);
	//}

	//for (int iSprite = 0; iSprite < m_EffectObj.size(); iSprite++)
	//{
	//	m_EffectObj[iSprite].Render();
	//}

	/*if (m_TimerObj.size() >= m_iTimerSprite)
	{
		m_TimerObj[m_iTimerSprite].Draw(300, 0, SRCAND);
		m_TimerObj[m_iTimerSprite].Draw(350, 0, SRCAND);
		m_TimerObj[m_iTimerSprite].Draw(400, 0, SRCAND);
	}*/

	for (int i = 0; i < m_SpliteListClick.size(); i++)
	{
		m_SpliteListClick[i].Render();
	}

	for (std::vector<TSprite>::iterator iter =
		m_SpliteListClick.begin();
		iter != m_SpliteListClick.end();)
	{
		if ((*iter).m_bDead)
		{
			iter = m_SpliteListClick.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool   TScriptMgr::Release() 
{
	for (int i = 0; i < m_TimerObj.size(); i++)
	{
		m_TimerObj[i].Release();
	}
	for (int i = 0; i < m_EffectObj.size(); i++)
	{
		m_EffectObj[i].Release();
	}
	for (int i = 0; i < m_UIObj.size(); i++)
	{
		m_UIObj[i].Release();
	}
	for (int i = 0; i < m_SpliteListClick.size(); i++)
	{
		m_SpliteListClick[i].Release();
	}
	return true;
}

TScriptMgr::TScriptMgr()
{
	m_iIndex = 0;
	m_iTimerSprite = 0;
	m_iNumberSprite = -1;;
	m_iObjectSprite = -1;
	m_iUISpriete = -1;
	Init();
}


TScriptMgr::~TScriptMgr()
{
	Release();
}
