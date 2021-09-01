// SampleMutexTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	HANDLE hMutex = OpenMutex(
		MUTEX_ALL_ACCESS, 
		FALSE, L"CHAT_MUTEX");
	ReleaseMutex(hMutex);
    return 0;
}

