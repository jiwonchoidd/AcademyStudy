#pragma once
#include "xSprite.h"

class TSpriteMgr : public TSingleton < TSpriteMgr >
{
private:
	friend class TSingleton<TSpriteMgr>;
	HDC			m_hScreenDC;
	HDC			m_hOffScreenDC;
public:
	typedef map <INT, xSprite*>					TemplateMap;
	typedef TemplateMap::iterator				TemplateMapItor;	
	TemplateMapItor								TItor;
	TemplateMap									TMap;
	INT											m_iCurIndex;
	

public:
	void			SetDevice(HDC  hdc, HDC offDC ); 
	INT				Add(	TCHAR *pszSpriteName,
							TCHAR *pszColorFileName=0,
							TCHAR *pszMaskFileName=0	);
	xSprite*		GetPtr( INT iIndex );
	bool			Release();
public:	
	TSpriteMgr();
	~TSpriteMgr();
};
#define I_Sprite TSpriteMgr::GetInstance()