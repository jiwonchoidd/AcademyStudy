#pragma once
#include "TServerObj.h"

class TDebugString : public TSingleton<TDebugString>, public TServerObj
{
private:
	friend class TSingleton<TDebugString>;
public:
	HWND	  m_hList;
public:
	void		Print( char* fmt, ... );
public:
	bool		Init();
	bool		Release();
	WCHAR* GetMbToWcs(const char* strMsg);
	char* GetWcsToMbs(const WCHAR* strMsg);
	HRESULT ConvertWideStringToAnsiCch( char* msg,
							const WCHAR* strMsg, int iRen );
	HRESULT ConverAnsiStringToWideCch( WCHAR* msg, 
									const char* strMsg, 
									int nRen );
	void	T_ERROR(bool bPrint=false);
public:
	TDebugString(void);
	virtual ~TDebugString(void);
};
#define I_Debug TDebugString::GetInstance()