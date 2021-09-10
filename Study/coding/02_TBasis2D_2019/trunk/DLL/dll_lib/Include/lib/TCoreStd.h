#pragma once
#include <Windows.h>//mmsystem.h�ڵ������
#include <tchar.h>
// timeGetTime(); ����Ϸ��� 
// mmsystem.h �� winmm.lib �����ؾ� �Ѵ�.

template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};