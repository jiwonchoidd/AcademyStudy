#pragma once
#include <windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include <list>
//��� Ŭ�������� �����Ҽ��ְ� �ܺκ����� ����
extern float g_fSecPerFrame;
extern HWND  g_hWnd;
extern RECT  g_rtClient;


//���ø��� �̿��� �̱��� ����������
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