/*---------------------------------------------------------------

��Ŷ ó�� ���� �Լ�.

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
// ����� ����/���� �˸�.
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
	// ���ο� ����� ������ ó���Ѵ�.
	//-----------------------------------------------------
	gameCreatePlayer(dwSessionID);



	//-----------------------------------------------------
	// ������ �ִ� ����ڵ��� ����� �������ش�.
	//-----------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		//-----------------------------------------------------
		// ����� ����� ��� �̾Ƽ�, ��Ŷ�� ����� �ش� ����ڿ��� ���� !
		// ��, ��� ������ ����� ���Դ� ������ �ʴ´�.
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
	// ����� �����, ��� ���� ����ڸ� ã�Ƽ� ��������.
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
	// ����� �������� ��Ŷ�� ���� ��ο��� �Ѹ���.
	//-----------------------------------------------------
	mpDeleteCharacter(&clPacket, dwSessionID);

	//-----------------------------------------------------
	// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket);

	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
// �� ��Ŷ Ÿ�Կ� ���� ��Ŷ ó�� �Լ�.
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
	// ID �� ĳ���͸� �˻��Ѵ�.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);

	CAyaPacket clPacket;

	//-----------------------------------------------------
	// ������ ����.  ���۹�ȣ��, ���Ⱚ�� ����.
	//-----------------------------------------------------
	stpCharacter->dwAction = byDirection;


	//-----------------------------------------------------
	// ������ ��ġ�� ���� ��Ŷ�� ��ġ���� �ʹ� ū ���̰� ���ٸ�
	// Ŭ���̾�Ʈ�� ������ üũ.  ���������.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;

	//-----------------------------------------------------
	// ������ ����.
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
	// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
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
	// ID �� ĳ���͸� �˻��Ѵ�.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);
	
	if ( NULL == stpCharacter )
		return FALSE;

	//-----------------------------------------------------
	// ������ ��ġ�� ���� ��Ŷ�� ��ġ���� �ʹ� ū ���̰� ���ٸ�
	// Ŭ���̾�Ʈ�� ������ üũ.  ���������.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;


	//-----------------------------------------------------
	// ������ ����.
	//-----------------------------------------------------
	stpCharacter->byDirection = byDirection;
	stpCharacter->dwAction = dfACTION_NONE;
	stpCharacter->shX = shX;
	stpCharacter->shY = shY;

	//-----------------------------------------------------
	// ���� ������ ��� Ŭ���̾�Ʈ���� ����.
	//-----------------------------------------------------
	CAyaPacket clPacket;

	mpMoveStop(&clPacket, dwSessionID, stpCharacter->byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
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
	// ID �� ĳ���͸� �˻��Ѵ�.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);
	
	if ( NULL == stpCharacter )
		return FALSE;

	//-----------------------------------------------------
	// ������ ��ġ�� ���� ��Ŷ�� ��ġ���� �ʹ� ū ���̰� ���ٸ�
	// Ŭ���̾�Ʈ�� ������ üũ.  ���������.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;

	//-----------------------------------------------------
	// ������ ����.
	//-----------------------------------------------------
	stpCharacter->byDirection = byDirection;
	stpCharacter->dwAction = dfACTION_ATTACK1;
	stpCharacter->shX = shX;
	stpCharacter->shY = shY;


    //-----------------------------------------------------
	// ���� ������ ��� Ŭ���̾�Ʈ���� ����.
	//-----------------------------------------------------
	CAyaPacket clPacket;
	mpAttack1(&clPacket, dwSessionID, stpCharacter->byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket, dwSessionID);



	//-----------------------------------------------------
	// �´� �༮�� �˻�����.
	//-----------------------------------------------------
	dwDamageSessionID = gameAttackCheck(1, dwSessionID);
	st_CHARACTER *stpDamageCharacter = gameGetCharacter(dwDamageSessionID);

	if ( NULL != stpDamageCharacter )
	{
		//-----------------------------------------------------
		// ������ ����.
		//-----------------------------------------------------
		stpDamageCharacter->chHP = max(stpDamageCharacter->chHP - dfATTACK1_DAMAGE, 0);



		//-----------------------------------------------------
		// ������ ��Ŷ ����� ����.
		//-----------------------------------------------------
		clPacket.Clear();
		mpDamage(&clPacket, dwSessionID, stpDamageCharacter->dwSessionID, stpDamageCharacter->chHP);

		//-----------------------------------------------------
		// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
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
	// ID �� ĳ���͸� �˻��Ѵ�.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);
	
	if ( NULL == stpCharacter )
		return FALSE;

	//-----------------------------------------------------
	// ������ ��ġ�� ���� ��Ŷ�� ��ġ���� �ʹ� ū ���̰� ���ٸ�
	// Ŭ���̾�Ʈ�� ������ üũ.  ���������.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;

	//-----------------------------------------------------
	// ������ ����.
	//-----------------------------------------------------
	stpCharacter->byDirection = byDirection;
	stpCharacter->dwAction = dfACTION_ATTACK1;
	stpCharacter->shX = shX;
	stpCharacter->shY = shY;


	//-----------------------------------------------------
	// ���� ������ ��� Ŭ���̾�Ʈ���� ����.
	//-----------------------------------------------------
	CAyaPacket clPacket;

	mpAttack2(&clPacket, dwSessionID, stpCharacter->byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket, dwSessionID);




	//-----------------------------------------------------
	// �´� �༮�� �˻�����.
	//-----------------------------------------------------
	dwDamageSessionID = gameAttackCheck(2, dwSessionID);
	st_CHARACTER *stpDamageCharacter = gameGetCharacter(dwDamageSessionID);

	if ( NULL != stpDamageCharacter )
	{
		//-----------------------------------------------------
		// ������ ����.
		//-----------------------------------------------------
		stpDamageCharacter->chHP = max(stpDamageCharacter->chHP - dfATTACK2_DAMAGE, 0);



		//-----------------------------------------------------
		// ������ ��Ŷ ����� ����.
		//-----------------------------------------------------
		clPacket.Clear();
		mpDamage(&clPacket, dwSessionID, stpDamageCharacter->dwSessionID, stpDamageCharacter->chHP);

		//-----------------------------------------------------
		// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
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
	// ID �� ĳ���͸� �˻��Ѵ�.
	//-----------------------------------------------------
	st_CHARACTER *stpCharacter = gameGetCharacter(dwSessionID);
	
	if ( NULL == stpCharacter )
		return FALSE;

	//-----------------------------------------------------
	// ������ ��ġ�� ���� ��Ŷ�� ��ġ���� �ʹ� ū ���̰� ���ٸ�
	// Ŭ���̾�Ʈ�� ������ üũ.  ���������.
	//-----------------------------------------------------
	if ( abs(stpCharacter->shX - shX) > dfERROR_RANGE || abs(stpCharacter->shY - shY) > dfERROR_RANGE )
		return FALSE;

	//-----------------------------------------------------
	// ������ ����.
	//-----------------------------------------------------
	stpCharacter->byDirection = byDirection;
	stpCharacter->dwAction = dfACTION_ATTACK1;
	stpCharacter->shX = shX;
	stpCharacter->shY = shY;


	//-----------------------------------------------------
	// ���� ������ ��� Ŭ���̾�Ʈ���� ����.
	//-----------------------------------------------------
	CAyaPacket clPacket;

	mpAttack3(&clPacket, dwSessionID, stpCharacter->byDirection, stpCharacter->shX, stpCharacter->shY);

	//-----------------------------------------------------
	// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
	//-----------------------------------------------------
	netSendPacketAll(&clPacket, dwSessionID);


	//-----------------------------------------------------
	// �´� �༮�� �˻�����.
	//-----------------------------------------------------
	dwDamageSessionID = gameAttackCheck(3, dwSessionID);
	st_CHARACTER *stpDamageCharacter = gameGetCharacter(dwDamageSessionID);

	if ( NULL != stpDamageCharacter )
	{
		//-----------------------------------------------------
		// ������ ����.
		//-----------------------------------------------------
		stpDamageCharacter->chHP = max(stpDamageCharacter->chHP - dfATTACK3_DAMAGE, 0);



		//-----------------------------------------------------
		// ������ ��Ŷ ����� ����.
		//-----------------------------------------------------
		clPacket.Clear();
		mpDamage(&clPacket, dwSessionID, stpDamageCharacter->dwSessionID, stpDamageCharacter->chHP);

		//-----------------------------------------------------
		// ���� �������� ����ڿ��� ��� ��Ŷ�� �Ѹ���.
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

