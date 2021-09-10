#pragma once
#include "TSprite.h"

struct TSpriteData
{
	T_STR	m_szPath;
	T_STR	m_szName;
	int		m_iIndex;
	int     m_iBitmapID;
	int     m_iMaskBitmapID;
	std::vector<TObjectInfo>   m_SpliteListData;
};
class TScriptMgr : public TSingleton<TScriptMgr>
{
	friend class TSingleton<TScriptMgr>;
	typedef std::map<int, TSpriteData>::iterator ITOR;
	std::map<int, TSpriteData>   m_list;
	int			m_iIndex;
public:
	int			m_iNumberSprite;
	int			m_iObjectSprite;
	int			m_iUISpriete;
	int         m_iTimerSprite;
public:
	std::vector<TSprite>   m_TimerObj;
	std::vector<TSprite>   m_EffectObj;
	std::vector<TSprite>   m_UIObj;
	std::vector<TSprite>   m_SpliteListClick;
public:
	int     Load(const TCHAR* pFileName);
	bool	GameDataLoad(T_STR name, TSpriteData& data);
	TSpriteData*  GetPtr(int iKey);
	bool    Init();
	bool    Frame();
	bool    Render();
	bool    Release();
private:
	TScriptMgr();
public:
	virtual ~TScriptMgr();
};
#define I_ScriptManager TScriptMgr::GetInstance()
