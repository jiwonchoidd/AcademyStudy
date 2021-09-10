

#include <winsock2.h>
#include <windows.h>

#include "AyaStreamSQ.h"
#include "AyaPacket.h"
#include "Network.h"

#include "GameProc.h"

#include "main.h"


/////////////////////////////////////////////////////////////////////
// 서버초기화.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////
BOOL InitServer(void)
{
	gameInitCharacter();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////
// 서버삭제.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////
BOOL ReleaseServer(void)
{


	return TRUE;
}



/////////////////////////////////////////////////////////////////////
// 메인 함수.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////
void main(void)
{
	timeBeginPeriod( 1 );

	//---------------------------------------------------------------
	// 윈속 스타트 업.
	//---------------------------------------------------------------
	if ( !netStartUp() )
		goto END;

	if ( !InitServer() )
		goto END;

	//---------------------------------------------------------------
	// 서버 포트 바인딩 및 리슨
	//---------------------------------------------------------------
	if ( !netPortOpen() )
		goto END;
	

	DWORD	dwOldTick = timeGetTime();
	DWORD	dwNowTick = 0;
	DWORD	dwUseTick = 0;

	while ( 1 )
	{
		//---------------------------------------------------------------
		// 네트워크 처리.
		//---------------------------------------------------------------
		if ( !netNetworkProc() )
			break;

		//---------------------------------------------------------------
		// 게임처리.  50프레임 맞춤.
		//---------------------------------------------------------------
		dwNowTick = timeGetTime();
		dwUseTick = dwNowTick - dwOldTick;

		if ( dwUseTick >= 20 )
		{
			dwOldTick = dwNowTick - (dwUseTick - 20);
			gameRun();
		}

		if ( GetAsyncKeyState(VK_F12) & 0x8000 )
		{
			break;
		}

	}


END:

	netCleanUp();
	ReleaseServer();

	timeEndPeriod( 1 );

}


