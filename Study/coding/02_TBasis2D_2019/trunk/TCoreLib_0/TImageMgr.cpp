#include "TImageMgr.h"

int TImageMgr::Load( HWND hWnd, 
					 HDC hdc,
					 TCHAR_STRING fileName )
{
	// 중복 제거
	TCHAR Dirve[MAX_PATH] = {0, };
	TCHAR Dir[MAX_PATH] = {0, };
	TCHAR Name[MAX_PATH] = {0, };
	TCHAR Ext[MAX_PATH] = {0, };
	TCHAR szFileName[MAX_PATH] = {0, };

	if( !fileName.empty() )
	{
		_tsplitpath( fileName.c_str(),
			Dirve, Dir, Name, Ext );
		_stprintf_s( szFileName, _T("%s%s"),
			Name, Ext );
		
		TImage* pImage;
		for( TItor itor = m_Map.begin(); 
		 itor != m_Map.end();
		 itor++ )
		{
			pImage = (TImage*)(*itor).second;
			
			if( !_tcsicmp(pImage->m_strName.c_str(),
				szFileName ))
			{
				return (*itor).first;
			}
		}


	}
	TImage* pImage = new TImage;
	if( pImage == NULL ) return -1;
	pImage->Init();
	pImage->m_strName = szFileName;
	pImage->Load( hWnd, hdc, fileName );

	m_Map.insert( make_pair(++m_iCurIndex,
						    pImage) );
	return m_iCurIndex;

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
