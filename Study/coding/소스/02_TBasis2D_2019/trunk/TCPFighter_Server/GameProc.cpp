
/*---------------------------------------------------------------

TCPFighter ������ ����ó�� �κ� �Լ�.

---------------------------------------------------------------*/
#include <windows.h>
#include "AyaPacket.h"
#include "AyaStreamSQ.h"
#include "Network.h"

#include "PacketProc.h"
#include "PacketDefine.h"

#include "MakePacket.h"
#include "GameProc.h"


//-----------------------------------------------------
// ĳ���� ���� ����ü.
//-----------------------------------------------------
st_CHARACTER	g_stCharacter[dfNETWORK_MAX_USER];


////////////////////////////////////////////////////////////////////
// ĳ���� ���� �ʱ�ȭ.
//
// Parameters: ����.
// Return: ����.
////////////////////////////////////////////////////////////////////
void	gameInitCharacter(void)
{
	memset(g_stCharacter, 0, sizeof(st_CHARACTER) * dfNETWORK_MAX_USER);
}


////////////////////////////////////////////////////////////////////
// ����� ����ID �� �˻�.
//
// Parameters: (DWORD)SessionID.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
st_CHARACTER	*gameGetCharacter(DWORD dwSessionID)
{
	int				iCount = 0;

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( dwSessionID == g_stCharacter[iCount].dwSessionID )
		{
			return &g_stCharacter[iCount];
		}
	}
	return NULL;
}


////////////////////////////////////////////////////////////////////
// ���ο� ����� ����.
//
// Parameters: (DWORD)SessionID.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	gameCreatePlayer(DWORD dwSessionID)
{
	st_CHARACTER 	*stpCharacter = NULL;
	int				iCount = 0;

	CAyaPacket		clPacket;

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		//-----------------------------------------------------
		// ����ִ� ĳ���� ������ ã�Ƽ� ����.
		//-----------------------------------------------------
		if ( !g_stCharacter[iCount].bFlag )
		{
			//-----------------------------------------------------
			// ĳ���� ���� ���� �� ��ǥ ����.
			//-----------------------------------------------------
			g_stCharacter[iCount].bFlag = TRUE;
			g_stCharacter[iCount].dwSessionID = dwSessionID;

			g_stCharacter[iCount].shX = (rand() % 580) + 40;
			g_stCharacter[iCount].shY = (rand() % 360) + 100;

			g_stCharacter[iCount].byDirection = dfPACKET_MOVE_DIR_LL;

			g_stCharacter[iCount].dwAction = dfACTION_NONE;


			g_stCharacter[iCount].chHP = 100;


			//-----------------------------------------------------
			// ĳ���� ���ο��Դ� �ڱ� ĳ���� ���� ��Ŷ��,
			// �ٸ� Ŭ���̾�Ʈ�� ���Դ� Ÿ�� ĳ���� ���� ��Ŷ��.
			//-----------------------------------------------------
			mpCreateMyCharacter(&clPacket,	g_stCharacter[iCount].dwSessionID,
											g_stCharacter[iCount].byDirection,
											g_stCharacter[iCount].shX,
											g_stCharacter[iCount].shY,
											g_stCharacter[iCount].chHP);

			netSendPacket(g_stCharacter[iCount].dwSessionID, &clPacket);
			LogOutput("Create Character # SessionID:%d    X:%d    Y:%d", g_stCharacter[iCount].dwSessionID,
																		 g_stCharacter[iCount].shX,
																		 g_stCharacter[iCount].shY);


			clPacket.Clear();
			mpCreateOtherCharacter(&clPacket,	g_stCharacter[iCount].dwSessionID,
												g_stCharacter[iCount].byDirection,
												g_stCharacter[iCount].shX,
												g_stCharacter[iCount].shY,
												g_stCharacter[iCount].chHP);

			netSendPacketAll(&clPacket, g_stCharacter[iCount].dwSessionID);
			
			return TRUE;
		}
	}

	return FALSE;
}



////////////////////////////////////////////////////////////////////
// �̵����� ���� üũ. 
//
// Parameters: (int)X. (int)Y.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	gameMoveCheck(int iX, int iY)
{
	if ( dfRANGE_MOVE_LEFT < iX && dfRANGE_MOVE_RIGHT > iX &&
		 dfRANGE_MOVE_TOP < iY && dfRANGE_MOVE_BOTTOM > iY )
		 return TRUE;

	return FALSE;
}



////////////////////////////////////////////////////////////////////
// ���� �浹 üũ. 
// �Էµ� ĳ���Ϳ� ����, �Ÿ��� ����Ͽ� �浹üũ�� ��.
//
// Parameters: (DWORD)������ SessionID.
// Return: (int)������ SessionID.
////////////////////////////////////////////////////////////////////
DWORD	gameAttackCheck(int iAttackType, DWORD dwSessionID)
{
	st_CHARACTER	*stpAttackCharacter = gameGetCharacter(dwSessionID);
	st_CHARACTER	*stpDamageCharacter;

	int iCount;

	//-----------------------------------------------------
	// ĳ���͸� �˻��ϸ� ���� �浹 üũ�� �Ѵ�.
	//-----------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		stpDamageCharacter = &g_stCharacter[iCount];

		//-----------------------------------------------------
		// �ڱ� �ڽŰ��� ������ �ʾƾ� �Ѵ�.
		//-----------------------------------------------------
		if ( stpDamageCharacter->bFlag && stpDamageCharacter->dwSessionID != dwSessionID )
		{
			if ( dfATTACK1_RANGE_Y > abs(stpDamageCharacter->shY - stpAttackCharacter->shY) )
			{
				switch ( iAttackType )
				{
				case 1:
					switch ( stpAttackCharacter->byDirection )
					{
					case dfPACKET_MOVE_DIR_LL:
						if ( dfATTACK1_RANGE_X >= stpAttackCharacter->shX - stpDamageCharacter->shX &&
							0 <= stpAttackCharacter->shX - stpDamageCharacter->shX )
							 
							return stpDamageCharacter->dwSessionID;

						break;


					case dfPACKET_MOVE_DIR_RR:
						if ( dfATTACK1_RANGE_X >= stpDamageCharacter->shX - stpAttackCharacter->shX &&
							0 <= stpDamageCharacter->shX - stpAttackCharacter->shX )
							 
							return stpDamageCharacter->dwSessionID;

						break;
					}
					break;


				case 2:
					switch ( stpAttackCharacter->byDirection )
					{
					case dfPACKET_MOVE_DIR_LL:
						if ( dfATTACK2_RANGE_X >= stpAttackCharacter->shX - stpDamageCharacter->shX &&
							0 <= stpAttackCharacter->shX - stpDamageCharacter->shX )
							 
							return stpDamageCharacter->dwSessionID;

						break;

					case dfPACKET_MOVE_DIR_RR:
						if ( dfATTACK2_RANGE_X >= stpDamageCharacter->shX - stpAttackCharacter->shX &&
							0 <= stpDamageCharacter->shX - stpAttackCharacter->shX )
							 
							return stpDamageCharacter->dwSessionID;

						break;
					}
					break;


				case 3:
					switch ( stpAttackCharacter->byDirection )
					{
					case dfPACKET_MOVE_DIR_LL:
						if ( dfATTACK3_RANGE_X >= stpAttackCharacter->shX - stpDamageCharacter->shX &&
							0 <= stpAttackCharacter->shX - stpDamageCharacter->shX )
							 
							return stpDamageCharacter->dwSessionID;

						break;

					case dfPACKET_MOVE_DIR_RR:
						if ( dfATTACK3_RANGE_X >= stpDamageCharacter->shX - stpAttackCharacter->shX &&
							0 <= stpDamageCharacter->shX - stpAttackCharacter->shX )
							 
							return stpDamageCharacter->dwSessionID;

						break;
					}
				}
			}	
		}
	}
	return 0xffffffff;
}



////////////////////////////////////////////////////////////////////
// ������ü ���μ���.
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	gameRun(void)
{
	st_CHARACTER 	*stpCharacter = NULL;
	int				iCount = 0;

	//-----------------------------------------------------
	// ĳ���� ������ ã�Ƽ� ó���Ѵ�.
	//-----------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( g_stCharacter[iCount].bFlag )
		{
			stpCharacter = &g_stCharacter[iCount];

			if ( 0 >= stpCharacter->chHP )
			{
				//-----------------------------------------------------
				// ���ó��.
				//-----------------------------------------------------
				netRemoveSession(stpCharacter->dwSessionID);
			}
			else
			{
				//-----------------------------------------------------
				// ���� ���ۿ� ���� ó��.
				//-----------------------------------------------------
				switch ( stpCharacter->dwAction )
				{
				case dfACTION_MOVE_LL:
					if ( gameMoveCheck(stpCharacter->shX - dfSPEED_PLAYER_X, stpCharacter->shY) )
					{
						stpCharacter->shX -= dfSPEED_PLAYER_X;
						LogOutput("# gameRun:LL # SessionID:%d / X:%d / Y:%d", stpCharacter->dwSessionID, stpCharacter->shX, stpCharacter->shY);
					}

					break;

				case dfACTION_MOVE_LU:
					if ( gameMoveCheck(stpCharacter->shX - dfSPEED_PLAYER_X, stpCharacter->shY - dfSPEED_PLAYER_Y) )
					{
						stpCharacter->shX -= dfSPEED_PLAYER_X;
						stpCharacter->shY -= dfSPEED_PLAYER_Y;
						LogOutput("# gameRun:LU # SessionID:%d / X:%d / Y:%d", stpCharacter->dwSessionID, stpCharacter->shX, stpCharacter->shY);
					}
					break;

				case dfACTION_MOVE_UU:
					if ( gameMoveCheck(stpCharacter->shX, stpCharacter->shY - dfSPEED_PLAYER_Y) )
					{
						stpCharacter->shY -= dfSPEED_PLAYER_Y;
						LogOutput("# gameRun:UU # SessionID:%d / X:%d / Y:%d", stpCharacter->dwSessionID, stpCharacter->shX, stpCharacter->shY);
					}
					break;

				case dfACTION_MOVE_RU:
					if ( gameMoveCheck(stpCharacter->shX + dfSPEED_PLAYER_X, stpCharacter->shY - dfSPEED_PLAYER_Y) )
					{
						stpCharacter->shX += dfSPEED_PLAYER_X;
						stpCharacter->shY -= dfSPEED_PLAYER_Y;
						LogOutput("# gameRun:RU # SessionID:%d / X:%d / Y:%d", stpCharacter->dwSessionID, stpCharacter->shX, stpCharacter->shY);
					}
					break;

				case dfACTION_MOVE_RR:
					if ( gameMoveCheck(stpCharacter->shX + dfSPEED_PLAYER_X, stpCharacter->shY) )
					{
						stpCharacter->shX += dfSPEED_PLAYER_X;
						LogOutput("# gameRun:RR # SessionID:%d / X:%d / Y:%d", stpCharacter->dwSessionID, stpCharacter->shX, stpCharacter->shY);
					}
					break;

				case dfACTION_MOVE_RD:
					if ( gameMoveCheck(stpCharacter->shX + dfSPEED_PLAYER_X, stpCharacter->shY + dfSPEED_PLAYER_Y) )
					{
						stpCharacter->shX += dfSPEED_PLAYER_X;
						stpCharacter->shY += dfSPEED_PLAYER_Y;
						LogOutput("# gameRun:RD # SessionID:%d / X:%d / Y:%d", stpCharacter->dwSessionID, stpCharacter->shX, stpCharacter->shY);
					}
					break;

				case dfACTION_MOVE_DD:
					if ( gameMoveCheck(stpCharacter->shX, stpCharacter->shY + dfSPEED_PLAYER_Y) )
					{
						stpCharacter->shY += dfSPEED_PLAYER_Y;
						LogOutput("# gameRun:DD # SessionID:%d / X:%d / Y:%d", stpCharacter->dwSessionID, stpCharacter->shX, stpCharacter->shY);
					}
					break;

				case dfACTION_MOVE_LD:
					if ( gameMoveCheck(stpCharacter->shX - dfSPEED_PLAYER_X, stpCharacter->shY + dfSPEED_PLAYER_Y) )
					{
						stpCharacter->shX -= dfSPEED_PLAYER_X;
						stpCharacter->shY += dfSPEED_PLAYER_Y;
						LogOutput("# gameRun:LD # SessionID:%d / X:%d / Y:%d", stpCharacter->dwSessionID, stpCharacter->shX, stpCharacter->shY);
					}
					break;
				}
			}
		}
	}
	return TRUE;
}
