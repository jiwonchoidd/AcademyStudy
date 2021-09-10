#pragma once
#include "TStd.h"

class TDebugString: public TSingleton<TDebugString>
{
private:
	friend class TSingleton<TDebugString>;
public:
	CRITICAL_SECTION	m_CS;
	std::list<T_STR>  m_Msg;
public:
	TCHAR_STRING_VECTOR	m_Message;
	HWND			m_hEdit;
	void		DisplayText( const char* fmt, ...);
	TCHAR*		GetMbtoWcs( const char* srcmsg );
	HRESULT		ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 
											   int cchDestChar );
	HRESULT		ConvertAnsiStringToWideCch( WCHAR* wstrDestination, 
										const CHAR* strSource, 
										int cchDestChar );
public:
	bool		Init();
	bool		Frame();
	bool		Release();
	void		T_ERROR(bool bPrint=true);

public:
	TDebugString(void);
	~TDebugString(void);
};
#define I_DebugStr TDebugString::GetInstance()