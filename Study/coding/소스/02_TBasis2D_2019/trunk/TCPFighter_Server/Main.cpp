

#include <winsock2.h>
#include <windows.h>

#include "AyaStreamSQ.h"
#include "AyaPacket.h"
#include "Network.h"

#include "GameProc.h"

#include "main.h"


/////////////////////////////////////////////////////////////////////
// �����ʱ�ȭ.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////
BOOL InitServer(void)
{
	gameInitCharacter();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////
// ��������.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////
BOOL ReleaseServer(void)
{


	return TRUE;
}



/////////////////////////////////////////////////////////////////////
// ���� �Լ�.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////
void main(void)
{
	timeBeginPeriod( 1 );

	//---------------------------------------------------------------
	// ���� ��ŸƮ ��.
	//---------------------------------------------------------------
	if ( !netStartUp() )
		goto END;

	if ( !InitServer() )
		goto END;

	//---------------------------------------------------------------
	// ���� ��Ʈ ���ε� �� ����
	//---------------------------------------------------------------
	if ( !netPortOpen() )
		goto END;
	

	DWORD	dwOldTick = timeGetTime();
	DWORD	dwNowTick = 0;
	DWORD	dwUseTick = 0;

	while ( 1 )
	{
		//---------------------------------------------------------------
		// ��Ʈ��ũ ó��.
		//---------------------------------------------------------------
		if ( !netNetworkProc() )
			break;

		//---------------------------------------------------------------
		// ����ó��.  50������ ����.
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


