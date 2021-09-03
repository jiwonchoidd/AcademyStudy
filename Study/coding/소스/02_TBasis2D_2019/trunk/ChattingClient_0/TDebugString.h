#pragma once
#include "TServerobj.h"

class TDebugString: public TSingleton<TDebugString>, public TServerObj
{
private:
	friend class TSingleton<TDebugString>;
public:
	TCHAR_STRING_VECTOR	m_Message;
	HWND			m_hEdit;
	HWND			m_hList;
	HWND			m_hButton;
	void		DisplayText( char* fmt, ...);
	void		Print( char* fmt, ...);

	TCHAR*		GetMbtoWcs( const char* srcmsg );
	char*		GetWcstoMbs( WCHAR* srcmsg );
	HRESULT		ConvertAnsiStringToWideCch( WCHAR* wstrDestination, 
										const CHAR* strSource, 
										int cchDestChar );
	HRESULT		ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 
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