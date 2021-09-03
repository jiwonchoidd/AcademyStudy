#pragma once
#include <Windows.h>//mmsystem.h�ڵ������
#include <tchar.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
// timeGetTime(); ����Ϸ��� 
// mmsystem.h �� winmm.lib �����ؾ� �Ѵ�.
// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� ����� 
// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� _EXPORTS ��ȣ��
// �����ϵǸ�, �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������ 
// TCOREDLL_API �Լ��� DLL���� �������� ������ ����, �� DLL��
// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.
#ifdef TCOREDLL_EXPORTS
#define TCOREDLL_API __declspec(dllexport)
#else
#define TCOREDLL_API __declspec(dllimport)
#endif

template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};