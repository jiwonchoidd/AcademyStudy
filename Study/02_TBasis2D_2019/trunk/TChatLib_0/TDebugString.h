#pragma once
#include "TServerObj.h"

class TDebugString 
{
public:
	WCHAR* GetMbToWcs(const char* strMsg);
	char* GetWcsToMbs(const WCHAR* strMsg);
	HRESULT ConvertWideStringToAnsiCch( char* msg,
							const WCHAR* strMsg, int iRen );
	HRESULT ConverAnsiStringToWideCch( WCHAR* msg, 
									const char* strMsg, 
									int nRen );
	void	T_ERROR(bool bPrint=false);
public:
	TDebugString(void);
	~TDebugString(void);
};