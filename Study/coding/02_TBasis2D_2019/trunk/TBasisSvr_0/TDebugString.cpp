#include "TDebugString.h"
#include "TBasisSys.h"
#include "TSynchronize.h"

void TDebugString::T_ERROR(bool bPrint)
{
	if( WSAGetLastError() != WSA_IO_PENDING )
	{
		setlocale(LC_ALL,"KOREAN");
		char* lpMsgBuf;
		FormatMessageA( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER|
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);
		if(bPrint)
			I_DebugStr.DisplayText("ERROR WSASend:%s\r\n",(char*)lpMsgBuf );				
		else
			OutputDebugStringA((char*)lpMsgBuf) ;
		LocalFree(lpMsgBuf);		
	}
}

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
	{
		TSynchronize sync( this );		
		va_list arg;
		va_start(arg, fmt );	
			char buf[1024+256] = {0,};
			vsprintf( buf, fmt, arg );	
			SendMessage( m_hEdit,EM_REPLACESEL,FALSE,(LPARAM)GetMbtoWcs(buf) );
		va_end(arg );		
	}
}
bool TDebugString::Init()
{
	//InitializeCriticalSection(&m_CS);
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
	//DeleteCriticalSection(&m_CS);	
	return true;
}
TDebugString::TDebugString(void)
{
}

TDebugString::~TDebugString(void)
{
}
