#pragma once
#include <assert.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <windows.h>
#include "KMath.h"
#include "dxtoolkit/SimpleMath.h"
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#pragma comment (lib, "D3DCompiler.lib")
#pragma comment	(lib, "d3d11.lib")
#pragma comment		(lib, "dxgi.lib")
#pragma comment (lib, "EngineCore.lib")
#pragma comment	(lib, "DirectXTK.lib")

using namespace DirectX::SimpleMath;

#pragma region wWinMain 매크로

//기본 매크로
#define WinSTART int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow){
#define WinRUN Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L"라이브러리"); g_Sample.Run();}
#define WinMain WinSTART WinRUN

//스트링화연산자를 이용한 매크로
#define WinRUN_OPT(NAME, WIDTH, HEIGHT) Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L#NAME, WIDTH, HEIGHT); g_Sample.Run();}
#define WinMain_OPT(NAME, WIDTH, HEIGHT) WinSTART WinRUN_OPT(NAME, WIDTH, HEIGHT)
//인풋 매크로

#pragma endregion

extern float g_fTimer;
extern float g_fSecPerFrame;
extern HWND  g_hWnd;
extern RECT  g_rtClient;
extern ID3D11Device* g_pd3dDevice;		// 디바이스 객체 외부변수

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