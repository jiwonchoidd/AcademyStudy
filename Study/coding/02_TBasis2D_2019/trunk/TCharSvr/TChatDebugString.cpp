#include "TChatDebugString.h"

void TChatDebugString::Print( char* fmt, ... )
{
	va_list arg;
	va_start(arg, fmt );
		char buf[256] = {0,};
		vsprintf( buf, fmt, arg );
		SendMessage( I_Debug.m_hList, LB_ADDSTRING, 0, 
			(LPARAM)I_Debug.GetMbToWcs(buf) );
	va_end(arg);
}
TChatDebugString::TChatDebugString(void)
{
}


TChatDebugString::~TChatDebugString(void)
{
}
