
/*---------------------------------------------------------------

TCPFighter 서버의 게임처리 부분 함수.

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
// 캐릭터 정보 구조체.
//-----------------------------------------------------
st_CHARACTER	g_stCharacter[dfNETWORK_MAX_USER];


////////////////////////////////////////////////////////////////////
// 캐릭터 정보 초기화.
//
// Parameters: 없음.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	gameInitCharacter(void)
{
	memset(g_stCharacter, 0, sizeof(st_CHARACTER) * dfNETWORK_MAX_USER);
}


////////////////////////////////////////////////////////////////////
// 사용자 세션ID 로 검색.
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
// 새로운 사용자 생성.
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
		// 비어있는 캐릭터 정보를 찾아서 넣자.
		//-----------------------------------------------------
		if ( !g_stCharacter[iCount].bFlag )
		{
			//-----------------------------------------------------
			// 캐릭터 정보 저장 및 좌표 생성.
			//-----------------------------------------------------
			g_stCharacter[iCount].bFlag = TRUE;
			g_stCharacter[iCount].dwSessionID = dwSessionID;

			g_stCharacter[iCount].shX = (rand() % 580) + 40;
			g_stCharacter[iCount].shY = (rand() % 360) + 100;

			g_stCharacter[iCount].byDirection = dfPACKET_MOVE_DIR_LL;

			g_stCharacter[iCount].dwAction = dfACTION_NONE;


			g_stCharacter[iCount].chHP = 100;


			//-----------------------------------------------------
			// 캐릭터 주인에게는 자기 캐릭터 생성 패킷을,
			// 다른 클라이언트들 에게는 타인 캐릭터 생성 패킷을.
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
// 이동가능 여부 체크. 
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
// 공격 충돌 체크. 
// 입력된 캐릭터와 방향, 거리를 사용하여 충돌체크를 함.
//
// Parameters: (DWORD)공격자 SessionID.
// Return: (int)데미지 SessionID.
////////////////////////////////////////////////////////////////////
DWORD	gameAttackCheck(int iAttackType, DWORD dwSessionID)
{
	st_CHARACTER	*stpAttackCharacter = gameGetCharacter(dwSessionID);
	st_CHARACTER	*stpDamageCharacter;

	int iCount;

	//-----------------------------------------------------
	// 캐릭터를 검색하며 공격 충돌 체크를 한다.
	//-----------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		stpDamageCharacter = &g_stCharacter[iCount];

		//-----------------------------------------------------
		// 자기 자신과는 비교하지 않아야 한다.
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
// 게임전체 프로세싱.
//
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	gameRun(void)
{
	st_CHARACTER 	*stpCharacter = NULL;
	int				iCount = 0;

	//-----------------------------------------------------
	// 캐릭터 정보를 찾아서 처리한다.
	//-----------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( g_stCharacter[iCount].bFlag )
		{
			stpCharacter = &g_stCharacter[iCount];

			if ( 0 >= stpCharacter->chHP )
			{
				//-----------------------------------------------------
				// 사망처리.
				//-----------------------------------------------------
				netRemoveSession(stpCharacter->dwSessionID);
			}
			else
			{
				//-----------------------------------------------------
				// 현재 동작에 따른 처리.
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
