
/*---------------------------------------------------------------

TCPFighter 서버의 게임처리 부분 함수.

---------------------------------------------------------------*/

#ifndef __GAME_PROC__
#define __GAME_PROC__

//-----------------------------------------------------------------
// 캐릭터 동작 플래그
//-----------------------------------------------------------------
#define dfACTION_MOVE_LL		0
#define dfACTION_MOVE_LU		1
#define dfACTION_MOVE_UU		2
#define dfACTION_MOVE_RU		3
#define dfACTION_MOVE_RR		4
#define dfACTION_MOVE_RD		5
#define dfACTION_MOVE_DD		6
#define dfACTION_MOVE_LD		7
#define dfACTION_ATTACK1		8
#define dfACTION_ATTACK2		9
#define dfACTION_ATTACK3		10
#define dfACTION_DAMAGE			11
#define dfACTION_NONE			255


//---------------------------------------------------------------
// 공격범위.
//---------------------------------------------------------------
#define dfATTACK1_RANGE_X		80
#define dfATTACK2_RANGE_X		90
#define dfATTACK3_RANGE_X		100
#define dfATTACK1_RANGE_Y		10
#define dfATTACK2_RANGE_Y		10
#define dfATTACK3_RANGE_Y		20


//---------------------------------------------------------------
// 공격 데미지.
//---------------------------------------------------------------
#define dfATTACK1_DAMAGE		1
#define dfATTACK2_DAMAGE		2
#define dfATTACK3_DAMAGE		3


//-----------------------------------------------------------------
// 캐릭터 이동 속도
//-----------------------------------------------------------------
#define dfSPEED_PLAYER_X	3
#define dfSPEED_PLAYER_Y	2

//-----------------------------------------------------------------
// 화면 이동 범위.
//-----------------------------------------------------------------
#define dfRANGE_MOVE_TOP	50
#define dfRANGE_MOVE_LEFT	10
#define dfRANGE_MOVE_RIGHT	630
#define dfRANGE_MOVE_BOTTOM	470


//-----------------------------------------------------------------
// 이동 오류체크 범위
//-----------------------------------------------------------------
#define dfERROR_RANGE		50


//---------------------------------------------------------------
// 캐릭터 정보 구조체.
//---------------------------------------------------------------
struct st_CHARACTER
{
	BOOL	bFlag;
	DWORD	dwSessionID;

	DWORD	dwAction;
	BYTE	byDirection;

	short	shX;
	short	shY;

	char	chHP;
};


extern st_CHARACTER		g_stCharacter[dfNETWORK_MAX_USER];



////////////////////////////////////////////////////////////////////
// 캐릭터 정보 초기화.
//
// Parameters: 없음.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void	gameInitCharacter(void);

////////////////////////////////////////////////////////////////////
// 사용자 세션ID 로 검색.
//
// Parameters: (DWORD)SessionID.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
st_CHARACTER	*gameGetCharacter(DWORD dwSessionID);



////////////////////////////////////////////////////////////////////
// 새로운 사용자 생성.
//
// Parameters: (DWORD)SessionID.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	gameCreatePlayer(DWORD dwSessionID);



////////////////////////////////////////////////////////////////////
// 공격 충돌 체크. 
// 입력된 캐릭터와 방향, 거리를 사용하여 충돌체크를 함.
//
// Parameters: (DWORD)공격자 SessionID.
// Return: (int)데미지 SessionID.
////////////////////////////////////////////////////////////////////
DWORD	gameAttackCheck(int iAttackType, DWORD dwSessionID);


////////////////////////////////////////////////////////////////////
// 이동가능 여부 체크. 
//
// Parameters: (int)X. (int)Y.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	gameMoveCheck(int iX, int iY);



////////////////////////////////////////////////////////////////////
// 게임전체 프로세싱.
//
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	gameRun(void);


#endif

