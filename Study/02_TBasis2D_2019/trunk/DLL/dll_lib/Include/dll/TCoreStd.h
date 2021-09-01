#pragma once
#include <Windows.h>//mmsystem.h자동연결됨
#include <tchar.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
// timeGetTime(); 사용하려면 
// mmsystem.h 과 winmm.lib 연결해야 한다.
// 다음 ifdef 블록은 DLL에서 내보내기하는 작업을 쉽게 해 주는 매크로를 만드는 
// 표준 방식입니다. 이 DLL에 들어 있는 파일은 모두 명령줄에 정의된 _EXPORTS 기호로
// 컴파일되며, 다른 프로젝트에서는 이 기호를 정의할 수 없습니다.
// 이렇게 하면 소스 파일에 이 파일이 들어 있는 다른 모든 프로젝트에서는 
// TCOREDLL_API 함수를 DLL에서 가져오는 것으로 보고, 이 DLL은
// 이 DLL은 해당 매크로로 정의된 기호가 내보내지는 것으로 봅니다.
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