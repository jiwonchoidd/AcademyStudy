#pragma once
#include "TShape.h"
class TBitmapMgr : public TSingleton<TBitmapMgr>
{
private:
	friend class TSingleton<TBitmapMgr>;
	/*static TBitmapMgr& GetInstance()
	{
		static TBitmapMgr thisSingleton;
		return thisSingleton;
	};*/

public:
	int			m_iCounter;
	std::map<int, TBitmap*>  m_List;
public:
	int    Add(TCHAR* strName);
	TBitmap*  GetPtr(int iIndex);
	bool	Release();
private:
	TBitmapMgr();
	//TBitmapMgr::TBitmapMgr': private 멤버('TBitmapMgr' 클래스에서 선언)에 액세스할 수 없습니다.
public:
	virtual ~TBitmapMgr();
};
#define I_BMgr TBitmapMgr::GetInstance()
