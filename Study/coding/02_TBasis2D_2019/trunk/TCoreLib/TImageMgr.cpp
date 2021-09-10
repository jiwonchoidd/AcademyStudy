#include "TImageMgr.h"
int    TImageMgr::CreateLoad(LONG iWidth, LONG iHeight,  LONG bitCount, T_STR FilePath)
{
	if (FilePath.empty()) return -1;

	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(FilePath.c_str(),	Drive, Dir, Name, Ext);

	T_STR filename = Name;
	filename += Ext;

	T_STR path = Drive;
	path += Dir;

	// 중복제거
	for (TItor itor = m_Map.begin();
		itor != m_Map.end();
		itor++)
	{
		TImage* pData = (*itor).second;
		if (pData->m_szName == filename)
		{
			return pData->m_iIndex;
		}
	}
	TImage* pData = new TImage;
	pData->Init();
	if (pData->CreateDIB(iWidth, iHeight))
	{
		pData->m_szName = filename;
		pData->m_szPath = path;
		pData->m_iIndex = ++m_iCurIndex;
		m_Map.insert(make_pair(pData->m_iIndex, pData));
		return pData->m_iIndex;
	}
	delete pData;
	return -1;
}
int        TImageMgr::Load(T_STR FilePath)
{
	// fullpath split
	if (FilePath.empty()) return -1;

	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(FilePath.c_str(),
		Drive, Dir, Name, Ext);

	T_STR filename = Name;
	filename += Ext;

	T_STR path = Drive;
	path += Dir;

	// 중복제거
	for (TItor itor = m_Map.begin();
		itor != m_Map.end();
		itor++)
	{
		TImage* pData = (*itor).second;
		if (pData->m_szName == filename)
		{
			return pData->m_iIndex;
		}
	}
	TImage* pData = new TImage;
	if (pData->Load(FilePath) == false)
	{
		delete pData;
		return -1;
	}
	pData->m_szName = filename;
	pData->m_szPath = path;
	pData->m_iIndex = ++m_iCurIndex;
	m_Map.insert(make_pair(pData->m_iIndex, pData));
	return pData->m_iIndex;
}

int TImageMgr::Load( HWND hWnd, 
					 HDC hdc,
	T_STR fileName )
{
	// 중복 제거
	TCHAR Dirve[MAX_PATH] = {0, };
	TCHAR Dir[MAX_PATH] = {0, };
	TCHAR Name[MAX_PATH] = {0, };
	TCHAR Ext[MAX_PATH] = {0, };
	TCHAR szFileName[MAX_PATH] = {0, };

	if( !fileName.empty() )
	{
		_tsplitpath_s( fileName.c_str(),
			Dirve, Dir, Name, Ext );
		_stprintf_s( szFileName, _T("%s%s"),
			Name, Ext );
		
		TImage* pImage;
		for( TItor itor = m_Map.begin(); 
		 itor != m_Map.end();
		 itor++ )
		{
			pImage = (TImage*)(*itor).second;
			
			if( !_tcsicmp(pImage->m_szName.c_str(),
				szFileName ))
			{
				return (*itor).first;
			}
		}


	}
	TImage* pImage = new TImage;
	if( pImage == NULL ) return -1;
	pImage->Init();
	pImage->m_szName = szFileName;
	pImage->m_iIndex = ++m_iCurIndex;
	pImage->Load( hWnd, hdc, fileName );

	m_Map.insert( make_pair(pImage->m_iIndex,   pImage) );
	return pImage->m_iIndex;

}
TImage* TImageMgr::GetPtr( int iIndex )
{
	TItor itor = m_Map.find( iIndex );
	if( itor == m_Map.end() ) return NULL;
	TImage* pImage = (*itor).second;
	return pImage;
}
TImageMgr::TImageMgr(void)
{
	m_iCurIndex = 0;
}

bool	TImageMgr::Init()
{
	return true;
}

bool	TImageMgr::Frame(){
	return true;
}
bool	TImageMgr::Render(){
	return true;
}
bool	TImageMgr::Release(){	
	TImage* pImage;
	for( TItor itor = m_Map.begin(); 
		 itor != m_Map.end();
		 itor++ )
	{
		pImage = (TImage*)(*itor).second;
		pImage->Release();
		delete pImage;
	}
	m_Map.clear();
	return true;
}
TImageMgr::~TImageMgr(void)
{
}
