#pragma once

#include <windows.h>
extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;
extern RECT			g_rtClient;
extern int			g_iFPS;// 1초당 프레임 개수
extern float		g_fSecPerFrame; // 1프레임의 경과시간
extern float		g_fAccumulation;// 실행된 경과 시간