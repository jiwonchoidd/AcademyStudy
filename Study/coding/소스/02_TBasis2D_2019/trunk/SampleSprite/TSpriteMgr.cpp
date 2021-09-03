#include "TSpriteMgr.h"

INT TSpriteMgr::Add(	TCHAR *pszSpriteName, 
						TCHAR *pszColorFileName,
						TCHAR *pszMaskFileName)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];	
	if(pszSpriteName)
	{										
		_tsplitpath_s(pszSpriteName, Drive, Dir, FName, Ext );
		Ext[4]= 0;	
		memset( szFileName, 0, sizeof(TCHAR) * MAX_PATH );
		_stprintf_s( szFileName, _T("%s%s"), FName, Ext );		

		for( TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++) 
		{
			xSprite *pPoint = (xSprite *)(*itor).second;
			if( !_tcsicmp( pPoint->m_szName.c_str(), szFileName ) ) 
			{
				return (*itor).first;
			}
		}
	}
	xSprite *pPoint = NULL;
	SAFE_NEW( pPoint, xSprite );
	pPoint->m_szPath = Dir;
	if( FAILED( pPoint->Load(m_hScreenDC, m_hOffScreenDC, pszColorFileName, pszMaskFileName  ) ))
	{
		SAFE_DEL( pPoint );
		return 0;
	}	
	pPoint->m_szName = szFileName;
	TMap.insert( make_pair( ++m_iCurIndex, pPoint ) );	
	pPoint->m_iIndex = m_iCurIndex;
	return m_iCurIndex;
}
bool TSpriteMgr::Release()
{
	xSprite *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++ ) 
	{
		pPoint = (xSprite *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;
	return true;
}
xSprite* TSpriteMgr::GetPtr( INT iIndex )
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return NULL;
	xSprite *pPoint = (*itor).second;	
	return pPoint;
}
void TSpriteMgr::SetDevice(HDC hdc, HDC offDC)
{
	m_hScreenDC = hdc;
	m_hOffScreenDC = offDC;
}
TSpriteMgr::TSpriteMgr(void)
{
	m_hScreenDC = NULL;
	m_iCurIndex = 0;	
	TMap.clear();
}

TSpriteMgr::~TSpriteMgr(void)
{
	xSprite *pPoint;
	for( TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++ ) 
	{
		pPoint = (xSprite *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;	
}
