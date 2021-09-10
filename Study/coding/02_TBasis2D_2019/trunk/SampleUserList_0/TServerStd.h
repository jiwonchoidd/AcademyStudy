#pragma once
#include <windows.h>
#include <string>
#include <list>
using namespace std;
typedef std::basic_string<TCHAR>   T_STR;
typedef std::basic_string<CHAR>   C_STR;

template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};