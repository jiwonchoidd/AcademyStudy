#pragma once
#include "TImage.h"
class TImageMgr : public TSingleton<TImageMgr>
{
private:
	friend class TSingleton<TImageMgr>;
public:
	typedef std::map<int,TImage*>::iterator TItor;
	map<int, TImage*>   m_Map;
	int					m_iCurIndex;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	int	  Load(	HWND hWnd, 
				HDC hdc, 
				TCHAR_STRING strLoadFile);
	TImage*    GetPtr( int iIndex );

private:
	TImageMgr(void);
	~TImageMgr(void);
};
#define I_ImageMgr TImageMgr::GetInstance()
