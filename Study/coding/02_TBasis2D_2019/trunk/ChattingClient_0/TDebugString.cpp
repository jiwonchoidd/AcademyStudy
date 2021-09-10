#include "TDebugString.h"
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
			I_DebugStr.Print("ERROR WSASend:%s",(char*)lpMsgBuf );				
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
	TCHAR msg[4096] = {0, };
	ConvertAnsiStringToWideCch(msg, srcmsg, strlen(srcmsg) );
	return msg;
}
HRESULT TDebugString::ConvertWideStringToAnsiCch( CHAR* strDestination, const WCHAR* wstrSource, 
										  int cchDestChar )
{
	if( strDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	int nResult = WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
		cchDestChar*sizeof(CHAR), NULL, NULL );
	strDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}
char* TDebugString::GetWcstoMbs( WCHAR* srcmsg )
{
	// 멀티바이트 => 유니코드 변환.
	char msg[1024] = {0, };
	ConvertWideStringToAnsiCch( msg,srcmsg, 1024);
	return msg;
}
void TDebugString::Print( char* fmt, ...)
{	
	{
		TSynchronize sync( this );	
		va_list arg;
		va_start(arg, fmt );	
			char buf[256] = {0,};
			vsprintf( buf, fmt, arg );	
			SendMessage( m_hList,LB_ADDSTRING,0,(LPARAM)GetMbtoWcs(buf) );
		va_end(arg );				
	}
}
void TDebugString::DisplayText( char* fmt, ...)
{	
	{
		TSynchronize sync( this );	
		va_list arg;
		va_start(arg, fmt );	
			char buf[256] = {0,};
			vsprintf( buf, fmt, arg );	
			int nLen=GetWindowTextLength(m_hEdit); 
			//SendMessage( m_hEdit, EM_SETSEL, nLen, nLen); 
			if( nLen > 256 )
			{
				SendMessage( m_hEdit, EM_SETSEL, 0, -1);            
				SendMessage( m_hEdit, WM_CLEAR, 0, 0);
			}
			SendMessage( m_hEdit,EM_REPLACESEL,0,(LPARAM)GetMbtoWcs(buf) );
		va_end(arg );		
	}
}

bool TDebugString::Init()
{	
	// 에디터 컨트롤 윈도우 생성
	m_hEdit = CreateWindow(	 L"edit", NULL,
							WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL,
							0,320,
							400,//this->m_iWidth,
							100,//this->m_iHeight,
							g_hWnd,(HMENU)0,
							g_hInstance,
							NULL );
	m_hButton = CreateWindow(	 L"button", L"전송",
							WS_CHILD | WS_VISIBLE ,
							403,320,
							90,//this->m_iWidth,
							100,//this->m_iHeight,
							g_hWnd,(HMENU)1,
							g_hInstance,
							NULL );

	// 에디터 컨트롤 윈도우 생성
	m_hList = CreateWindow(	 L"listbox", NULL,
							WS_CHILD | WS_VISIBLE |WS_HSCROLL|WS_VSCROLL, 
							/*| ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY
							| ES_AUTOHSCROLL,*/
							0,0,
							500,//this->m_iWidth,
							300,//this->m_iHeight,
							g_hWnd,(HMENU)2,
							g_hInstance,
							NULL );

	SendMessage( m_hList,LB_RESETCONTENT,0,0 );
	SendMessage( m_hList,LB_SETCOLUMNWIDTH, 100,0 );	
	return true;
}
bool TDebugString::Frame()
{
	return true;
}
bool TDebugString::Release()
{	
	return true;
}
TDebugString::TDebugString(void)
{
}

TDebugString::~TDebugString(void)
{
}
