/*---------------------------------------------------------------

패킷 처리 전용 함수.

---------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "AyaPacket.h"
#include "AyaStreamSQ.h"
#include "Network.h"
#include "MakePacket.h"

#include "GameProc.h"
#include "PacketDefine.h"





//////////////////////////////////////////////////////////////////////////
// 사용자 접속/해지 알림.
//
// Parameters: (DWORD)SessionID.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
BOOL	netPacketProc_Connect(DWORD dwSessionID)
{
	int			iCount;
	CAyaPacket	clPacket;

	LogOutput("# PACKET_CONNECT # SessionID:%d", dwSessionID);


	//-----------------------------------------------------
	// 새로운 사용자 접속을 처리한다.
	//-----------------------------------------------------
	gameCreatePlayer(dwSessionID);



	//-----------------------------------------------------
	// 기존에 있던 사용자들의 목록을 전송해준다.
	//-----------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		//-----------------------------------------------------
		// 사용자 목록을 모두 뽑아서, 패킷을 만들어 해당 사용자에게 전송 !
		// 단, 방금 접속한 사용자 에게는 보내지 않는다.
		//-----------------------------------------------------
		if ( g_stCharacter[iCount].bFlag && g_stCharacter[iCount].dwSessionID != dwSessionID )
		{
			clPacket.Clear();
			mpCreateOtherCharacter(&clPacket,
									g_stCharacter[iCount].dwSessionID,
									g_stCharacter[iCount].byDirection,
									g_stCharacter[iCount].shX,
									g_stCharacter[iCount].shY,
									g_stCharacter[iCount].chHP);
			

			netSendPacket(dwSessionID, &clPacket);
		}		
	}
	return TRUE;
}




BOOL	netPacketProc_Disconnect(DWORD dwSessionID)
{
	int			iCount;
	CAyaPacket	clPacket;

	//-----------------------------------------------------
	// 사용자 목록중, 방금 나간 사용자를 찾아서 지워주자.
	//-----------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( g_stCharacter[iCount].dwSessionID == dwSessionID )
		{
			g_stCharacter[iCount].bFlag = FALSE;
			g_stCharacter[iCount].dwSessionID = FALSE;

			g_stCharacter[iCount].shX = FALSE;
			g_stCharacter[iCount].shY = FALSE;

			g_stCharacter[iCount].chHP = FALSE;

			break;
		}		
	}

	//-----------------------------------------------------
	// 사용자 지워지는 패킷을 만들어서 모두에게 뿌린다.
	//-----------------------------------------------------
	mpDeleteCharacter(&clPacket, dwSessionID);

	//-----------------------------------------------------
	// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket);

	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
// 각 패킷 타입에 따른 패킷 처리 함수.
//
// Parameters: (DWORD)SessionID. (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
BOOL	netPacketProc_MoveStart(DWORD dwSessionID, CAyaPacket *clpPacket)
{
	BYTE byDirection=0;
	short shX, shY;

	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	LogOutput("# PACKET_MOVESTART # SessionID:%d / Direction:%d / X:%d / Y:%d", dwSessionID, byDirection, shX, shY);

	//-----------------------------------------------------
	// ID 로 캐릭터를 검색한다.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);

	CAyaPacket clPacket;

	//-----------------------------------------------------
	// 동작을 변경.  동작번호와, 방향값이 같다.
	//-----------------------------------------------------
	stpCharacter->dwAction = byDirection;


	//-----------------------------------------------------
	// 서버의 위치와 받은 패킷의 위치값이 너무 큰 차이가 난다면
	// 클라이언트의 문제로 체크.  끊어버린다.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;

	//-----------------------------------------------------
	// 방향을 변경.
	//-----------------------------------------------------
	switch ( byDirection )
	{
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		stpCharacter->byDirection = dfPACKET_MOVE_DIR_RR;
		break;

	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LD:
		stpCharacter->byDirection = dfPACKET_MOVE_DIR_LL;
		break;
	}




	stpCharacter->shX = shX;
	stpCharacter->shY = shY;

	mpMoveStart(&clPacket, dwSessionID, byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket, dwSessionID);


	return TRUE;
}


BOOL	netPacketProc_MoveStop(DWORD dwSessionID, CAyaPacket *clpPacket)
{
	BYTE byDirection;
	short shX, shY;

	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	LogOutput("# PACKET_MOVESTOP # SessionID:%d / Direction:%d / X:%d / Y:%d", dwSessionID, byDirection, shX, shY);

	//-----------------------------------------------------
	// ID 로 캐릭터를 검색한다.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);
	
	if ( NULL == stpCharacter )
		return FALSE;

	//-----------------------------------------------------
	// 서버의 위치와 받은 패킷의 위치값이 너무 큰 차이가 난다면
	// 클라이언트의 문제로 체크.  끊어버린다.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;


	//-----------------------------------------------------
	// 동작을 변경.
	//-----------------------------------------------------
	stpCharacter->byDirection = byDirection;
	stpCharacter->dwAction = dfACTION_NONE;
	stpCharacter->shX = shX;
	stpCharacter->shY = shY;

	//-----------------------------------------------------
	// 정지 내역을 모든 클라이언트에게 전송.
	//-----------------------------------------------------
	CAyaPacket clPacket;

	mpMoveStop(&clPacket, dwSessionID, stpCharacter->byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket, dwSessionID);

	return TRUE;
}


BOOL	netPacketProc_Attack1(DWORD dwSessionID, CAyaPacket *clpPacket)
{
	BYTE byDirection;
	DWORD dwDamageSessionID;
	short shX, shY;

	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	LogOutput("# PACKET_ATTACK1 # SessionID:%d / Direction:%d / X:%d / Y:%d", dwSessionID, byDirection, shX, shY);

	//-----------------------------------------------------
	// ID 로 캐릭터를 검색한다.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);
	
	if ( NULL == stpCharacter )
		return FALSE;

	//-----------------------------------------------------
	// 서버의 위치와 받은 패킷의 위치값이 너무 큰 차이가 난다면
	// 클라이언트의 문제로 체크.  끊어버린다.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;

	//-----------------------------------------------------
	// 동작을 변경.
	//-----------------------------------------------------
	stpCharacter->byDirection = byDirection;
	stpCharacter->dwAction = dfACTION_ATTACK1;
	stpCharacter->shX = shX;
	stpCharacter->shY = shY;


    //-----------------------------------------------------
	// 공격 내역을 모든 클라이언트에게 전송.
	//-----------------------------------------------------
	CAyaPacket clPacket;
	mpAttack1(&clPacket, dwSessionID, stpCharacter->byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket, dwSessionID);



	//-----------------------------------------------------
	// 맞는 녀석을 검색하자.
	//-----------------------------------------------------
	dwDamageSessionID = gameAttackCheck(1, dwSessionID);
	st_CHARACTER *stpDamageCharacter = gameGetCharacter(dwDamageSessionID);

	if ( NULL != stpDamageCharacter )
	{
		//-----------------------------------------------------
		// 에너지 감소.
		//-----------------------------------------------------
		stpDamageCharacter->chHP = max(stpDamageCharacter->chHP - dfATTACK1_DAMAGE, 0);



		//-----------------------------------------------------
		// 데미지 패킷 만들어 전송.
		//-----------------------------------------------------
		clPacket.Clear();
		mpDamage(&clPacket, dwSessionID, stpDamageCharacter->dwSessionID, stpDamageCharacter->chHP);

		//-----------------------------------------------------
		// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
		//-----------------------------------------------------
		netSendPacketAll(&clPacket);
	}
	return TRUE;
}


BOOL	netPacketProc_Attack2(DWORD dwSessionID, CAyaPacket *clpPacket)
{
	BYTE byDirection;
	DWORD dwDamageSessionID;
	short shX, shY;

	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	LogOutput("# PACKET_ATTACK2 # SessionID:%d / Direction:%d / X:%d / Y:%d", dwSessionID, byDirection, shX, shY);

	//-----------------------------------------------------
	// ID 로 캐릭터를 검색한다.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);
	
	if ( NULL == stpCharacter )
		return FALSE;

	//-----------------------------------------------------
	// 서버의 위치와 받은 패킷의 위치값이 너무 큰 차이가 난다면
	// 클라이언트의 문제로 체크.  끊어버린다.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;

	//-----------------------------------------------------
	// 동작을 변경.
	//-----------------------------------------------------
	stpCharacter->byDirection = byDirection;
	stpCharacter->dwAction = dfACTION_ATTACK1;
	stpCharacter->shX = shX;
	stpCharacter->shY = shY;


	//-----------------------------------------------------
	// 공격 내역을 모든 클라이언트에게 전송.
	//-----------------------------------------------------
	CAyaPacket clPacket;

	mpAttack2(&clPacket, dwSessionID, stpCharacter->byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket, dwSessionID);




	//-----------------------------------------------------
	// 맞는 녀석을 검색하자.
	//-----------------------------------------------------
	dwDamageSessionID = gameAttackCheck(2, dwSessionID);
	st_CHARACTER *stpDamageCharacter = gameGetCharacter(dwDamageSessionID);

	if ( NULL != stpDamageCharacter )
	{
		//-----------------------------------------------------
		// 에너지 감소.
		//-----------------------------------------------------
		stpDamageCharacter->chHP = max(stpDamageCharacter->chHP - dfATTACK2_DAMAGE, 0);



		//-----------------------------------------------------
		// 데미지 패킷 만들어 전송.
		//-----------------------------------------------------
		clPacket.Clear();
		mpDamage(&clPacket, dwSessionID, stpDamageCharacter->dwSessionID, stpDamageCharacter->chHP);

		//-----------------------------------------------------
		// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
		//-----------------------------------------------------
		netSendPacketAll(&clPacket);
	}

	return TRUE;
}


BOOL	netPacketProc_Attack3(DWORD dwSessionID, CAyaPacket *clpPacket)
{
	BYTE byDirection;
	DWORD dwDamageSessionID;
	short shX, shY;

	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	LogOutput("# PACKET_ATTACK3 # SessionID:%d / Direction:%d / X:%d / Y:%d", dwSessionID, byDirection, shX, shY);

	//-----------------------------------------------------
	// ID 로 캐릭터를 검색한다.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);
	
	if ( NULL == stpCharacter )
		return FALSE;

	//-----------------------------------------------------
	// 서버의 위치와 받은 패킷의 위치값이 너무 큰 차이가 난다면
	// 클라이언트의 문제로 체크.  끊어버린다.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;

	//-----------------------------------------------------
	// 동작을 변경.
	//-----------------------------------------------------
	stpCharacter->byDirection = byDirection;
	stpCharacter->dwAction = dfACTION_ATTACK1;
	stpCharacter->shX = shX;
	stpCharacter->shY = shY;


	//-----------------------------------------------------
	// 공격 내역을 모든 클라이언트에게 전송.
	//-----------------------------------------------------
	CAyaPacket clPacket;

	mpAttack3(&clPacket, dwSessionID, stpCharacter->byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket, dwSessionID);


	//-----------------------------------------------------
	// 맞는 녀석을 검색하자.
	//-----------------------------------------------------
	dwDamageSessionID = gameAttackCheck(3, dwSessionID);
	st_CHARACTER *stpDamageCharacter = gameGetCharacter(dwDamageSessionID);

	if ( NULL != stpDamageCharacter )
	{
		//-----------------------------------------------------
		// 에너지 감소.
		//-----------------------------------------------------
		stpDamageCharacter->chHP = max(stpDamageCharacter->chHP - dfATTACK3_DAMAGE, 0);



		//-----------------------------------------------------
		// 데미지 패킷 만들어 전송.
		//-----------------------------------------------------
		clPacket.Clear();
		mpDamage(&clPacket, dwSessionID, stpDamageCharacter->dwSessionID, stpDamageCharacter->chHP);

		//-----------------------------------------------------
		// 현재 접속중인 사용자에게 모든 패킷을 뿌린다.
		//-----------------------------------------------------
		netSendPacketAll(&clPacket);
	}

	return TRUE;
}




BOOL	netPacketProc_Sync(DWORD dwSessionID, CAyaPacket *clpPacket)
{
	LogOutput("# PACKET_SYNC # SessionID:%d", dwSessionID);

	return TRUE;
}

