#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>
#include<windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#pragma comment	(lib, "d3d11.lib")
#pragma comment		(lib, "dxgi.lib")
#pragma comment (lib, "EngineCore.lib")

#pragma region wWinMain ��ũ��

//�⺻ ��ũ��
#define WinSTART int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow){
#define WinRUN Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L"���̺귯��"); g_Sample.Run();}
#define WinMain WinSTART WinRUN

//��Ʈ��ȭ�����ڸ� �̿��� ��ũ��
#define WinRUN_OPT(NAME, WIDTH, HEIGHT) Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L#NAME, WIDTH, HEIGHT); g_Sample.Run();}
#define WinMain_OPT(NAME, WIDTH, HEIGHT) WinSTART WinRUN_OPT(NAME, WIDTH, HEIGHT)

#pragma endregion

extern float g_fSecPerFrame;
extern HWND  g_hWnd;
extern RECT  g_rtClient;
extern ID3D11Device* g_pd3dDevice;		// ����̽� ��ü
extern ID3D11DeviceContext* g_pImmediateContext;// ����̽� ���ؽ�Ʈ
//�̱���
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