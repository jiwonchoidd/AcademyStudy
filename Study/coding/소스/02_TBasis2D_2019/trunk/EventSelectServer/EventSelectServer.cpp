// EventSelectServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "TEventSelect.h"

TEventSelect   g_EventSelect;
int main()
{
	g_EventSelect.InitSelectServer();
	g_EventSelect.NetworkProcess();
	g_EventSelect.Release();
    return 0;
}

