#pragma once
#include<windows.h>
#include < tchar.h >
#include <string>
#include <map>
#include <vector>
#include <list>
#pragma comment (lib, "EngineCore.lib")

#pragma region wWinMain 매크로

//기본 매크로
#define WinSTART int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow){
#define WinRUN Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L"라이브러리"); g_Sample.Run();}
#define WinMain WinSTART WinRUN

//스트링화연산자를 이용한 매크로
#define WinRUN_OPT(NAME, WIDTH, HEIGHT) Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L#NAME, WIDTH, HEIGHT); g_Sample.Run();}
#define WinMain_OPT(NAME, WIDTH, HEIGHT) WinSTART WinRUN_OPT(NAME, WIDTH, HEIGHT)

#pragma endregion

extern float g_fSecPerFrame;
extern HWND  g_hWnd;
extern RECT  g_rtClient;

//싱글톤
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