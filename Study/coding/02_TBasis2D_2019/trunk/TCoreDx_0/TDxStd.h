#ifndef KGCA
	#define KGCA
	#pragma warning( disable: 4005)	
	#include <WinSock2.h>	
	#include "TUtils.h"
	#pragma comment( lib, "ws2_32.lib")
	#pragma comment( lib, "winmm.lib")
	#pragma comment(lib,"d3d11.lib")
	#pragma comment(lib,"dxgi.lib")	
	#pragma comment(lib,"d3dcompiler.lib")
#if defined(_DEBUG)
	#pragma comment(lib,"d3dx11d.lib")
	#pragma comment(lib,"d3dx10d.lib")	
#else
	#pragma comment(lib,"d3dx11.lib")
	#pragma comment(lib,"d3dx10.lib")	
#endif

#define TCORE_START int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR    lpCmdLine,int       nCmdShow){Sample sample;
#define TCORE_WIN(s,x,y) if (sample.SetWindow(hInstance, L#s, x, y))	{	sample.Run();	} }
#define TCORE_RUN(s,x,y) TCORE_START;TCORE_WIN(s,x,y);
#define TCORE_END }
	
#endif