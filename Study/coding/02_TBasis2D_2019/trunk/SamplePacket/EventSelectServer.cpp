// EventSelectServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "TEventSelect.h"
#include "TPacket.h"
TEventSelect   g_EventSelect;
struct TInfo
{
	int		iValue;
	float	fValue ;
	char	cValue;
	WORD	wValue;	
	CHAR	name[9];
};

int main()
{
	TPacket data;
	TInfo   info = { 1000, 999.3, 'A', 500,"홍길동" };
	C_STR	szName = "홍길동";
	// 출력
	data << info.name << szName << info.iValue << info.fValue << info.cValue << info.wValue;

	int		iReadValue = 0;
	float	fReadValue = 0;
	char	cReadValue = 0;
	WORD	wReadValue = 0;
	C_STR	strName;
	CHAR	name[9] = { 0, };
	// 입력
	TPacket read;
	memcpy(read.m_szBuffer, data.m_szBuffer, sizeof(data.m_szBuffer));
	read >> name >> strName >> iReadValue >> fReadValue >> cReadValue >> wReadValue;

	g_EventSelect.InitSelectServer();
	g_EventSelect.NetworkProcess();
	g_EventSelect.Release();
    return 0;
}

