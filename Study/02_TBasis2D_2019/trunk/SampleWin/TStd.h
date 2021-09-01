#ifndef KGCA
	#define KGCA
	#define	_CRT_SECURE_NO_WARNINGS
	#include <windows.h>
	#include <tchar.h>
	#include <vector>
	#include <string>
	using namespace std;
	#pragma comment( lib, "winmm.lib") //timeGetTime

	extern HWND			g_hWnd;
	extern RECT			g_rtClient;
	extern int			g_iFPS;// 1초당 프레임 개수
	extern float		g_fSecPerFrame; // 1프레임의 경과시간
	extern float		g_fAccumulation;// 실행된 경과 시간
#endif