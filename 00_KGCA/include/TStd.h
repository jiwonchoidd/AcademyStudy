#pragma once
#include<windows.h>
#include < tchar.h >
#include <string>
#include <map>
#include <vector>
#include <list>
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