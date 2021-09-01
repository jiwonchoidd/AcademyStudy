#include "TBitmapMgr.h"

int    TBitmapMgr::Add(TCHAR* strName)
{
	std::map<int, TBitmap*>::iterator itor;
	for (itor = m_List.begin();
		 itor != m_List.end();
		 itor++ )
	{
		TBitmap* data = (*itor).second;
		if (!_tcsicmp(strName, 
			data->m_szName.c_str()))
		{
			return (*itor).first;
		}
	}
	TBitmap* data = new TBitmap;
	data->Init();
	if (data->Load(strName))
	{
		m_List.insert( make_pair(m_iCounter++, data) );
		//m_List[m_iCounter++] = data;
		//m_List.insert( );
	}
	return m_iCounter-1;
};
TBitmap*  TBitmapMgr::GetPtr(int iKey)
{
	std::map<int, TBitmap*>::iterator itor;
	itor = m_List.find(iKey);
	if (itor == m_List.end()) return NULL;
	TBitmap* pBit = (*itor).second;
	return pBit;
}
bool TBitmapMgr::Release()
{
	std::map<int, TBitmap*>::iterator itor;
	for (itor = m_List.begin();
	itor != m_List.end();
		itor++)
	{
		TBitmap* data = (*itor).second;
		delete data;
	}
	m_List.clear();
	return true;
}
TBitmapMgr::TBitmapMgr()
{
	m_iCounter = 0;
}


TBitmapMgr::~TBitmapMgr()
{
	Release();
}
