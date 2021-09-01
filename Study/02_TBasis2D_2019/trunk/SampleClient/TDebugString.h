#pragma once
#include "TStd.h"

class TDebugString: public TSingleton<TDebugString>
{
private:
	friend class TSingleton<TDebugString>;
public:
	CRITICAL_SECTION	m_CS;
public:
	TCHAR_STRING_VECTOR	m_Message;
	HWND			m_hEdit;
	void		DisplayText( char* fmt, ...);
	WCHAR* GetMbToWcs(const char* strMsg);
	char* GetWcsToMbs(const WCHAR* strMsg);
	HRESULT ConvertWideStringToAnsiCch( char* msg,
							const WCHAR* strMsg, int iRen );
	HRESULT ConverAnsiStringToWideCch( WCHAR* msg, 
									const char* strMsg, 
									int nRen );
	void	T_ERROR(bool bPrint=false);
public:
	bool		Init();
public:
	TDebugString(void);
	~TDebugString(void);
};
#define I_DebugStr TDebugString::GetInstance()