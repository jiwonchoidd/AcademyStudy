// SampleMutexTest.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

