#pragma once
#include "TBasisStd.h"

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
	TCHAR*		GetMbtoWcs( const char* srcmsg );
	HRESULT		ConvertAnsiStringToWideCch( WCHAR* wstrDestination, 
										const CHAR* strSource, 
										int cchDestChar );
public:
	bool		Init();
	bool		Frame();
	bool		Release();

public:
	TDebugString(void);
	~TDebugString(void);
};
#define I_DebugStr TDebugString::GetInstance()