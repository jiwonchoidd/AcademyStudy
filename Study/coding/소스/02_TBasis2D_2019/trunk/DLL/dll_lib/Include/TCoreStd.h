#pragma once
#include <Windows.h>//mmsystem.h자동연결됨
#include <tchar.h>
// timeGetTime(); 사용하려면 
// mmsystem.h 과 winmm.lib 연결해야 한다.

template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};