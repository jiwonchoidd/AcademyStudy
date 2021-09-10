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
	int		Load(	HWND hWnd,HDC hdc, T_STR strLoadFile);
	int     Load(T_STR pFilePath);
	TImage*    GetPtr( int iIndex );
	int	    CreateLoad(LONG iWidth, LONG iHeight, LONG bitCount = 32, T_STR FilePath=_T(""));
private:
	TImageMgr(void);
public:
	~TImageMgr(void);
};
#define I_ImageMgr TImageMgr::GetInstance()
