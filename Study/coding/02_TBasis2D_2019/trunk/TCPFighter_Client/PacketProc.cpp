
/*---------------------------------------------------------------

��Ŷ ó�� ���� �Լ�.

---------------------------------------------------------------*/
#include "CommonInclude.h"

#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"

#include "DefineEnum.h"
#include "PacketDefine.h"

#include "Main.h"




//------------------------------------------------------------
// ������Ʈ ����Ʈ
// ȭ�鿡 �������� ��� ��ü���� �� ����Ʈ���� ó���ȴ�.
//------------------------------------------------------------
extern list<CBaseObject *> g_stlObjectList;

//------------------------------------------------------------
// �÷��̾� ������Ʈ�� ���� �ϳ��� ��ü�� �˾Ƶд�
//------------------------------------------------------------
extern CBaseObject *g_pPlayerObject;


//////////////////////////////////////////////////////////////////////////
// SessionID�� ��ü �˻�.
//
// Parameters: (DWORD)dwSessionID.
// Return: (CBaseObject *)��ü ������.
//////////////////////////////////////////////////////////////////////////
CBaseObject *FindPlayerObject(DWORD dwSessionID)
{
	list<CBaseObject *>::iterator listIter = g_stlObjectList.begin();
	CBaseObject *pObject;

	//------------------------------------------------------------
	// ��ü�� �˻��Ѵ�.
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
// �� ��Ŷ Ÿ�Կ� ���� ��Ŷ ó�� �Լ�.
//
// Parameters: (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////


// #define	dfPACKET_SC_CREATE_MY_CHARACTER			0
//---------------------------------------------------------------
// Ŭ���̾�Ʈ �ڽ��� ĳ���� �Ҵ�		Server -> Client
//
// ������ ���ӽ� ���ʷ� �ްԵǴ� ��Ŷ���� �ڽ��� �Ҵ���� ID ��
// �ڽ��� ���� ��ġ, HP �� �ް� �ȴ�. (ó���� �ѹ� �ް� ��)
// 
// �� ��Ŷ�� ������ �ڽ��� ID,X,Y,HP �� �����ϰ� ĳ���͸� �������Ѿ� �Ѵ�.
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
	// �ڽ��� ĳ���͸� �����ϰ� ������ ���� ��,
	// ����Ʈ�� ��Ͻ�Ų��.
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
// �ٸ� Ŭ���̾�Ʈ�� ĳ���� ���� ��Ŷ		Server -> Client
//
// ó�� ������ ���ӽ� �̹� ���ӵǾ� �ִ� ĳ���͵��� ����
// �Ǵ� �����߿� ���ӵ� Ŭ���̾�Ʈ���� ���� �� ����.
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
	// �ڽ��� ĳ���͸� �����ϰ� ������ ���� ��,
	// ����Ʈ�� ��Ͻ�Ų��.
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
// ĳ���� ���� ��Ŷ						Server -> Client
//
// ĳ������ �������� �Ǵ� ĳ���Ͱ� �׾����� ���۵�.
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
	// ��ü�� �˻��Ѵ�.
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
// ĳ���� �̵����� ��Ŷ						Server -> Client
//
// �ٸ� ������ ĳ���� �̵��� �� ��Ŷ�� �޴´�.
// ��Ŷ ���Ž� �ش� ĳ���͸� ã�� �̵�ó���� ���ֵ��� �Ѵ�.
// 
// ��Ŷ ���� �� �ش� Ű�� ����ؼ� ���������� �����ϰ�
// �ش� �������� ��� �̵��� �ϰ� �־�߸� �Ѵ�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ �� 8���� )
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
	// ��ü�� �˻��Ѵ�.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

	if ( NULL == clpObject )
		return FALSE;


	//------------------------------------------------------------
	// ��ǥ�� �����ϰ� ���� ���� �������� ����.
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
// ĳ���� �̵����� ��Ŷ						Server -> Client
//
// ID �� �ش��ϴ� ĳ���Ͱ� �̵��� ������̹Ƿ� 
// ĳ���͸� ã�Ƽ� �����, ��ǥ�� �Է����ְ� ���ߵ��� ó���Ѵ�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
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
	// ��ü�� �˻��Ѵ�.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

 	//------------------------------------------------------------
	// ����� ��ǥ�� �����ϰ� ���� ���� �������� ����.
	//------------------------------------------------------------
	((CPlayerObject *)clpObject)->SetDirection(byDirection);

	clpObject->SetPosition(shX, shY);
	clpObject->ActionInput(dfACTION_STAND);

	return TRUE;

}



// #define	dfPACKET_SC_ATTACK1						21
//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Server -> Client
//
// ��Ŷ ���Ž� �ش� ĳ���͸� ã�Ƽ� ����1�� �������� �׼��� �����ش�.
// ������ �ٸ� ��쿡�� �ش� �������� �ٲ� �� ���ش�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
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
	// ��ü�� �˻��Ѵ�.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

 	//------------------------------------------------------------
	// ����� ��ǥ�� �����ϰ� ���� ���� �������� ����.
	//------------------------------------------------------------
	((CPlayerObject *)clpObject)->SetDirection(byDirection);

	clpObject->SetPosition(shX, shY);
	clpObject->ActionInput(dfACTION_ATTACK1);

	return TRUE;
}

// #define	dfPACKET_SC_ATTACK2						23
//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Server -> Client
//
// ��Ŷ ���Ž� �ش� ĳ���͸� ã�Ƽ� ����2�� �������� �׼��� �����ش�.
// ������ �ٸ� ��쿡�� �ش� �������� �ٲ� �� ���ش�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
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
	// ��ü�� �˻��Ѵ�.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

 	//------------------------------------------------------------
	// ����� ��ǥ�� �����ϰ� ���� ���� �������� ����.
	//------------------------------------------------------------
	((CPlayerObject *)clpObject)->SetDirection(byDirection);

	clpObject->SetPosition(shX, shY);
	clpObject->ActionInput(dfACTION_ATTACK2);

	return TRUE;
}

// #define	dfPACKET_SC_ATTACK3						25
//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ							Server -> Client
//
// ��Ŷ ���Ž� �ش� ĳ���͸� ã�Ƽ� ����3�� �������� �׼��� �����ش�.
// ������ �ٸ� ��쿡�� �ش� �������� �ٲ� �� ���ش�.
//
//	4	-	ID
//	1	-	Direction	( ���� ������ ��. ��/�츸 ��� )
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
	// ��ü�� �˻��Ѵ�.
	//------------------------------------------------------------
	CBaseObject *clpObject = FindPlayerObject(dwSessionID);

 	//------------------------------------------------------------
	// ����� ��ǥ�� �����ϰ� ���� ���� �������� ����.
	//------------------------------------------------------------
	((CPlayerObject *)clpObject)->SetDirection(byDirection);

	clpObject->SetPosition(shX, shY);
	clpObject->ActionInput(dfACTION_ATTACK3);

	return TRUE;
}


// #define	dfPACKET_SC_DAMAGE						30
//---------------------------------------------------------------
// ĳ���� ������ ��Ŷ							Server -> Client
//
// ���ݿ� ���� ĳ������ ������ ����.
//
//	4	-	AttackID	( ������ ID )
//	4	-	DamageID	( ������ ID )
//	1	-	DamageHP	( ������ HP )
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
	// ��ü�� �˻��Ѵ�.
	//------------------------------------------------------------
	CBaseObject *clpAttackObject = FindPlayerObject(dwAttackSessionID);
	CBaseObject *clpDamageObject = FindPlayerObject(dwDamageSessionID);

	//------------------------------------------------------------
	// ���߿� �ϳ��� ���ٸ� �ȵ�����.
	//------------------------------------------------------------
	if ( NULL == clpAttackObject || NULL == clpDamageObject )
		return FALSE;

	//------------------------------------------------------------
	// ���� ĳ���� HP �� �����Ѵ�.
	//------------------------------------------------------------
	((CPlayerObject *)clpDamageObject)->SetHP(chHP);

	//------------------------------------------------------------
	// ����Ʈ ������Ʈ�� ��Ͻ�Ų��.
	//------------------------------------------------------------
	CEffectObject *clpEffect = new CEffectObject(eTYPE_EFFECT, 0xfffffffe, dwAttackSessionID);
	clpEffect->SetSprite(eEFFECT_SPARK_01, eEFFECT_SPARK_MAX, dfDELAY_EFFECT);


	//------------------------------------------------------------
	// ����Ʈ ������Ʈ�� ������ġ�� ���Ѵ�.
	// �����ڿ� �������� �߰����� �� �����.
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
// ����ȭ�� ���� ��Ŷ					Server -> Client
//
// �����κ��� ����ȭ ��Ŷ�� ������ �ش� ĳ���͸� ã�Ƽ�
// ĳ���� ��ǥ�� �������ش�.
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


