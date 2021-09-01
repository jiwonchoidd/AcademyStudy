#pragma once
#include "TBitmap.h"
class TBitmapManager : public TSingleton<TBitmapManager>
{
public:
	friend  class TSingleton<TBitmapManager>;
private:
	std::map<wstring, TBitmap*>				m_List;
	std::map<wstring, TBitmap*>::iterator	m_iter;
	std::wstring				m_szDefaultPath;
public:
	TBitmap*		CreateLoad(LONG iWidth, LONG iHeight, const TCHAR* filename, int iBitCount = 24);
	TBitmap*		Load(const TCHAR* filename);
	TBitmap*		GetPtr(wstring filename);
	static	HBITMAP CreateDIB(int w, int h, LPVOID* pPixelData=nullptr);
	static  bool	SaveFile(HBITMAP hBitmap, const TCHAR* strSaveFile);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	TBitmapManager();
public:
	~TBitmapManager();
};
#define g_BitmapMgr TBitmapManager::GetInstance()
