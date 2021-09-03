#include "TDebugString.h"
void TDebugString::T_ERROR(bool bPrint)
{
	if(WSAGetLastError() != WSA_IO_PENDING )
	{
		char* lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			WSAGetLastError(),
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL );
		if( bPrint )
		{
		}
		else
		{
			OutputDebugStringA((char*)lpMsgBuf);
		}
		LocalFree( lpMsgBuf );
	}
}
WCHAR* TDebugString::GetMbToWcs(const char* strMsg)
{
	TCHAR msg[4096] = {0, };
	int nRen = MultiByteToWideChar(CP_ACP, 0, strMsg,
		                     -1, 0, 0 );
	ConverAnsiStringToWideCch( msg, strMsg, nRen );
	return msg;
}
HRESULT TDebugString::ConverAnsiStringToWideCch( WCHAR* msg, 
									const char* strMsg, 
									int nRen )
{
	if( msg == NULL || strMsg == NULL || nRen < 1)
	{
		return E_INVALIDARG;
	}
	int nRetrun = MultiByteToWideChar(CP_ACP, 0, 
								strMsg, -1, 
								msg, nRen );
	if(nRen == 0) return E_FAIL;
	msg[nRen-1]=0;
	return S_OK;	
};

char* TDebugString::GetWcsToMbs(const WCHAR* strMsg)
{
	char msg[4096] = {0, };
	int iRen = WideCharToMultiByte(CP_ACP, 0, strMsg, -1, 0, 0, NULL, NULL); 
	ConvertWideStringToAnsiCch( msg, strMsg, iRen );
	return msg;
}
HRESULT TDebugString::ConvertWideStringToAnsiCch( char* msg,
							const WCHAR* strMsg, int iRen )
{
	if( msg == NULL || strMsg == NULL || iRen < 1 )
	{
		return E_INVALIDARG;
	}
	int iReturn = WideCharToMultiByte(CP_ACP, 0, 
		strMsg, -1, 
		msg, iRen, NULL, NULL); 
	if( iReturn == 0 ) return E_FAIL;
	return S_OK;
};
TDebugString::TDebugString(void)
{
}


TDebugString::~TDebugString(void)
{
}
