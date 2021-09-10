#pragma once
#include "TDebugString.h"
class TChatDebugString : public TSingleton<TChatDebugString>, public TDebugString
{
private:
	friend class TSingleton<TChatDebugString>;
public:
	HWND	  m_hList;
public:
	void		Print( char* fmt, ... );
public:
	TChatDebugString(void);
	~TChatDebugString(void);
};
#define I_Debug TChatDebugString::GetInstance()

