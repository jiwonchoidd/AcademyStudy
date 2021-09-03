
/*---------------------------------------------------------------

패킷 처리 전용 함수.

---------------------------------------------------------------*/
#include "CommonInclude.h"

#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"

#include "DefineEnum.h"
#include "PacketDefine.h"

#include "Main.h"




//------------------------------------------------------------
// 오브젝트 리스트
// 화면에 보여지는 모든 객체들은 이 리스트에서 처리된다.
//------------------------------------------------------------
extern list<CBaseObject *> g_stlObjectList;

//------------------------------------------------------------
// 플레이어 오브젝트에 대해 하나의 객체를 알아둔다
//------------------------------------------------------------
extern CBaseObject *g_pPlayerObject;


//////////////////////////////////////////////////////////////////////////
// SessionID로 객체 검색.
//
// Parameters: (DWORD)dwSessionID.
// Return: (CBaseObject *)객체 포인터.
//////////////////////////////////////////////////////////////////////////
CBaseObject *FindPlayerObject(DWORD dwSessionID)
{
	list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
	CBaseObject *pObject;

	//------------------------------------------------------------
	// 객체를 검색한다.
	//------------------------------------------------------------
	while ( g_stlObjectList.end() != listIter )
	{
		pObject = *listIter;
		if ( dwSessionID == pObject->GetObjectID() )
		{
			return pObject;
		}
		listIter++;
	}

	return NULL;
}






//////////////////////////////////////////////////////////////////////////
// 각 패킷 타입에 따른 패킷 처리 함수.
//
// Parameters: (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////


// #define	dfPACKET_SC_CREATE_MY_CHARACTER			0
//---------------------------------------------------------------
// 클라이언트 자신의 캐릭터 할당		Server -> Client
//
// 서버에 접속시 최초로 받게되는 패킷으로 자신이 할당받은 ID 와
// 자신의 최초 위치, HP 를 받게 된다. (처음에 한번 받게 됨)
// 
// 이 패킷을 받으면 자신의 ID,X,Y,HP 를 저장하고 캐릭터를 생성시켜야 한다.
//
//	4	-	ID
//	1	-	Direction
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------
BOOL	netPacketProc_CreateMyCharacter(CAyaPacket *clpPacket)
{
	DWORD dwSessionID;
	short shX, shY;
	char chHP;
	BYTE byDirection;

	*clpPacket >> dwSessionID;
	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;
	*clpPacket >> chHP;
	
	//------------------------------------------------------------
	// 자신의 캐릭터를 생성하고 포인터 저장 후,
	// 리스트에 등록시킨다.
	//------------------------------------------------------------
	g_pPlayerObject = new CPlayerObject(eTYPE_PLAYER, dwSessionID, TRUE);
	g_pPlayerObject->SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, dfDELAY_STAND);

	g_pPlayerObject->SetPosition(shX, shY);
	((CPlayerObject *)g_pPlayerObject)->SetDirection(byDirection);
	((CPlayerObject *)g_pPlayerObject)->SetHP(chHP);

	g_stlObjectList.push_back(g_pPlayerObject);

	return TRUE;
}


// #define	dfPACKET_SC_CREATE_OTHER_CHARACTER		1
//---------------------------------------------------------------
// 다른 클라이언트의 캐릭터 생성 패킷		Server -> Client
//
// 처음 서버에 접속시 이미 접속되어 있던 캐릭터들의 정보
// 또는 게임중에 접속된 클라이언트들의 생성 용 정보.
//
//
//	4	-	ID
//	1	-	Direction
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------
BOOL	netPacketProc_CreateOtherCharacter(CAyaPacket *clpPacket)
{
	DWORD dwSessionID;
	short shX, shY;
	char chHP;
	BYTE byDirection;

	*clpPacket >> dwSessionID;
	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;
	*clpPacket >> chHP;
	
	//------------------------------------------------------------
	// 자신의 캐릭터를 생성하고 포인터 저장 후,
	// 리스트에 등록시킨다.
	//------------------------------------------------------------
	CPlayerObject *clpObject = new CPlayerObject(eTYPE_PLAYER, dwSessionID, FALSE);
	clpObject->SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, dfDELAY_STAND);

	clpObject->SetPosition(shX, shY);
	clpObject->SetDirection(byDirection);
	clpObject->SetHP(chHP);

	g_stlObjectList.push_back(clpObject);

	return TRUE;
}


// #define	dfPACKET_SC_DELETE_CHARACTER			2
//---------------------------------------------------------------
// 캐릭터 삭제 패킷						Server -> Client
//
// 캐릭터의 접속해제 또는 캐릭터가 죽었을때 전송됨.
//
//	4	-	ID
//
//---------------------------------------------------------------
BOOL	netPacketProc_DeleteCharacter(CAyaPacket *clpPacket)
{
	DWORD	dwSessionID;
	*clpPacket >> dwSessionID;

	list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
	CBaseObject *pObject;

	//------------------------------------------------------------
	// 객체를 검색한다.
	//------------------------------------------------------------
	while ( g_stlObjectList.end() != listIter )
	{
		pObject = *listIter;
		if ( dwSessionID == pObject->GetObjectID() )
		{
			delete pObject;
			g_stlObjectList.erase(listIter);

			break;
		}
		listIter++;
	}

	return TRUE;
}


// #define	dfPACKET_SC_MOVE_START					11
//---------------------------------------------------------------
// 캐릭터 이동시작 패킷						Server -> Client
//
// 다른 유저의 캐릭터 이동시 본 패킷을 받는다.
// 패킷 수신시 해당 캐릭터를 찾아 이동처리를 해주도록 한다.
// 
// 패킷 수신 시 해당 키가 계속해서 눌린것으로 생각하고
// 해당 방향으로 계속 이동을 하고 있어야만 한다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값 8방향 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
/*
#define dfPACKET_MOVE_DIR_LL					0
#define dfPACKET_MOVE_DIR_LU					1
#define dfPACKET_MOVE_DIR_UU					2
#define dfPACKET_MOVE_DIR_RU					3
#define dfPACKET_MOVE_DIR_RR					4
#define dfPACKET_MOVE_DIR_RD					5
#define dfPACKET_MOVE_DIR_DD					6
#define dfPACKET_MOVE_DIR_LD					7
*/
BOOL	netPacketProc_MoveStart(CAyaPacket *clpPacket)
{
	DWORD	dwSessionID;
	BYTE	byDirection;
	short	shX;
	short	shY;

	*clpPacket >> dwSessionID;
	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	//------------------------------------------------------------
	// 객체를 검색한다.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

	if ( NULL == clpObject )
		return FALSE;


	//------------------------------------------------------------
	// 좌표를 설정하고 동을 정지 동작으로 변경.
	//------------------------------------------------------------
	clpObject->SetPosition(shX, shY);

	switch ( byDirection )
	{
	case dfPACKET_MOVE_DIR_LL:
		clpObject->ActionInput(dfACTION_MOVE_LL);
		break;

	case dfPACKET_MOVE_DIR_LU:
		clpObject->ActionInput(dfACTION_MOVE_LU);
		break;

	case dfPACKET_MOVE_DIR_UU:
		clpObject->ActionInput(dfACTION_MOVE_UU);
		break;

	case dfPACKET_MOVE_DIR_RU:
		clpObject->ActionInput(dfACTION_MOVE_RU);
		break;

	case dfPACKET_MOVE_DIR_RR:
		clpObject->ActionInput(dfACTION_MOVE_RR);
		break;

	case dfPACKET_MOVE_DIR_RD:
		clpObject->ActionInput(dfACTION_MOVE_RD);
		break;

	case dfPACKET_MOVE_DIR_DD:
		clpObject->ActionInput(dfACTION_MOVE_DD);
		break;

	case dfPACKET_MOVE_DIR_LD:
		clpObject->ActionInput(dfACTION_MOVE_LD);
		break;
	}

	return TRUE;
}


// #define	dfPACKET_SC_MOVE_STOP					13
//---------------------------------------------------------------
// 캐릭터 이동중지 패킷						Server -> Client
//
// ID 에 해당하는 캐릭터가 이동을 멈춘것이므로 
// 캐릭터를 찾아서 방향과, 좌표를 입력해주고 멈추도록 처리한다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
BOOL	netPacketProc_MoveStop(CAyaPacket *clpPacket)
{
	DWORD	dwSessionID;
	BYTE	byDirection;
	short	shX, shY;

	*clpPacket >> dwSessionID;
	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	//------------------------------------------------------------
	// 객체를 검색한다.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

 	//------------------------------------------------------------
	// 방향과 좌표를 설정하고 동을 정지 동작으로 변경.
	//------------------------------------------------------------
	((CPlayerObject *)clpObject)->SetDirection(byDirection);

	clpObject->SetPosition(shX, shY);
	clpObject->ActionInput(dfACTION_STAND);

	return TRUE;

}



// #define	dfPACKET_SC_ATTACK1						21
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격1번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
BOOL	netPacketProc_Attack1(CAyaPacket *clpPacket)
{
	DWORD	dwSessionID;
	BYTE	byDirection;
	short	shX, shY;

	*clpPacket >> dwSessionID;
	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	//------------------------------------------------------------
	// 객체를 검색한다.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

 	//------------------------------------------------------------
	// 방향과 좌표를 설정하고 동을 정지 동작으로 변경.
	//------------------------------------------------------------
	((CPlayerObject *)clpObject)->SetDirection(byDirection);

	clpObject->SetPosition(shX, shY);
	clpObject->ActionInput(dfACTION_ATTACK1);

	return TRUE;
}

// #define	dfPACKET_SC_ATTACK2						23
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격2번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
BOOL	netPacketProc_Attack2(CAyaPacket *clpPacket)
{
	DWORD	dwSessionID;
	BYTE	byDirection;
	short	shX, shY;

	*clpPacket >> dwSessionID;
	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	//------------------------------------------------------------
	// 객체를 검색한다.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

 	//------------------------------------------------------------
	// 방향과 좌표를 설정하고 동을 정지 동작으로 변경.
	//------------------------------------------------------------
	((CPlayerObject *)clpObject)->SetDirection(byDirection);

	clpObject->SetPosition(shX, shY);
	clpObject->ActionInput(dfACTION_ATTACK2);

	return TRUE;
}

// #define	dfPACKET_SC_ATTACK3						25
//---------------------------------------------------------------
// 캐릭터 공격 패킷							Server -> Client
//
// 패킷 수신시 해당 캐릭터를 찾아서 공격3번 동작으로 액션을 취해준다.
// 방향이 다를 경우에는 해당 방향으로 바꾼 후 해준다.
//
//	4	-	ID
//	1	-	Direction	( 방향 디파인 값. 좌/우만 사용 )
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
BOOL	netPacketProc_Attack3(CAyaPacket *clpPacket)
{
	DWORD	dwSessionID;
	BYTE	byDirection;
	short	shX, shY;

	*clpPacket >> dwSessionID;
	*clpPacket >> byDirection;
	*clpPacket >> shX;
	*clpPacket >> shY;

	//------------------------------------------------------------
	// 객체를 검색한다.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

 	//------------------------------------------------------------
	// 방향과 좌표를 설정하고 동을 정지 동작으로 변경.
	//------------------------------------------------------------
	((CPlayerObject *)clpObject)->SetDirection(byDirection);

	clpObject->SetPosition(shX, shY);
	clpObject->ActionInput(dfACTION_ATTACK3);

	return TRUE;
}


// #define	dfPACKET_SC_DAMAGE						30
//---------------------------------------------------------------
// 캐릭터 데미지 패킷							Server -> Client
//
// 공격에 맞은 캐릭터의 정보를 보냄.
//
//	4	-	AttackID	( 공격자 ID )
//	4	-	DamageID	( 피해자 ID )
//	1	-	DamageHP	( 피해자 HP )
//
//---------------------------------------------------------------
BOOL	netPacketProc_Damage(CAyaPacket *clpPacket)
{
	DWORD	dwAttackSessionID;
	DWORD	dwDamageSessionID;
	char	chHP;

	*clpPacket >> dwAttackSessionID;
	*clpPacket >> dwDamageSessionID;
	*clpPacket >> chHP;

	//------------------------------------------------------------
	// 객체를 검색한다.
	//------------------------------------------------------------
	CBaseObject *clpAttackObject = FindPlayerObject(dwAttackSessionID);
	CBaseObject *clpDamageObject = FindPlayerObject(dwDamageSessionID);

	//------------------------------------------------------------
	// 둘중에 하나라도 없다면 안되지롱.
	//------------------------------------------------------------
	if ( NULL == clpAttackObject || NULL == clpDamageObject )
		return FALSE;

	//------------------------------------------------------------
	// 맞은 캐릭터 HP 를 수정한다.
	//------------------------------------------------------------
	((CPlayerObject *)clpDamageObject)->SetHP(chHP);

	//------------------------------------------------------------
	// 이펙트 오브젝트를 등록시킨다.
	//------------------------------------------------------------
	CEffectObject *clpEffect = new CEffectObject(eTYPE_EFFECT, 0xfffffffe, dwAttackSessionID);
	clpEffect->SetSprite(eEFFECT_SPARK_01, eEFFECT_SPARK_MAX, dfDELAY_EFFECT);


	//------------------------------------------------------------
	// 이펙트 오브젝트의 생성위치를 정한다.
	// 공격자와 피해자의 중간점을 잘 골라보자.
	//------------------------------------------------------------
	short shEffectX, shEffectY;

	shEffectX = clpDamageObject->GetCurX();
	shEffectY = clpAttackObject->GetCurY() - 50;
	
	clpEffect->SetPosition(shEffectX, shEffectY);

	g_stlObjectList.push_back(clpEffect);

	return TRUE;
}

// #define	dfPACKET_SC_SYNC						251
//---------------------------------------------------------------
// 동기화를 위한 패킷					Server -> Client
//
// 서버로부터 동기화 패킷을 받으면 해당 캐릭터를 찾아서
// 캐릭터 좌표를 보정해준다.
//
//	4	-	ID
//	2	-	X
//	2	-	Y
//
//---------------------------------------------------------------
BOOL	netPacketProc_Sync(CAyaPacket *clpPacket)
{

	return TRUE;
}


