#include "TDebugString.h"
#include "TBasisSys.h"

HRESULT TDebugString::ConvertAnsiStringToWideCch( WCHAR* wstrDestination, 
									const CHAR* strSource, 
									int cchDestChar )
{
	if( wstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
										wstrDestination, cchDestChar );
	wstrDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}

TCHAR* TDebugString::GetMbtoWcs( const char* srcmsg )
{
	// 멀티바이트 => 유니코드 변환.
	TCHAR msg[256] = {0, };
	ConvertAnsiStringToWideCch(msg, srcmsg, 256);
	return msg;
}
void TDebugString::DisplayText( char* fmt, ...)
{
	EnterCriticalSection(&m_CS);	
		va_list arg;
		va_start(arg, fmt );	
			char buf[1024+256] = {0,};
			vsprintf( buf, fmt, arg );	
			SendMessage( m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)GetMbtoWcs(buf) );
		va_end(arg );
	LeaveCriticalSection(&m_CS);	
}
bool TDebugString::Init()
{
	InitializeCriticalSection(&m_CS);
	// 에디터 컨트롤 윈도우 생성
	m_hEdit = CreateWindow(	 L"edit", NULL,
							WS_CHILD | WS_VISIBLE |WS_HSCROLL|WS_VSCROLL 
							| ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY
							| ES_AUTOHSCROLL,
							0,0,
							500,//this->m_iWidth,
							500,//this->m_iHeight,
							g_hWnd,NULL,
							g_hInstance,
							NULL );

	
	return true;
}
bool TDebugString::Frame()
{
	return true;
}
bool TDebugString::Release()
{
	DeleteCriticalSection(&m_CS);	
	return true;
}
TDebugString::TDebugString(void)
{
}

TDebugString::~TDebugString(void)
{
}
