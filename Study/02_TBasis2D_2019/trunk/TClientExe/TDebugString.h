#pragma once
#include "TBasisStd.h"

class TDebugString: public TSingleton<TDebugString>
{
private:
	friend class TSingleton<TDebugString>;
public:
	CRITICAL_SECTION	m_CS;
	std::list<TCHAR_STRING>  m_Msg;
public:
	TCHAR_STRING_VECTOR	m_Message;
	HWND			m_hEdit;
	void		DisplayText( char* fmt, ...);
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