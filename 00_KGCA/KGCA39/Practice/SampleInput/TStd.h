#pragma once
#include <windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include <list>
//모든 클래스에서 접근할수있게 외부변수로 선언
extern float g_fSecPerFrame;
extern HWND  g_hWnd;
extern RECT  g_rtClient;


//탬플릿을 이용한 싱글톤 디자인패턴
template<class T>
class TSingleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};