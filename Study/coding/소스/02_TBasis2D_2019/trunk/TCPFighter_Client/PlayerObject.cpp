
/*---------------------------------------------------------------

CBaseObject 를 상속받은 플레이어 객체.


---------------------------------------------------------------*/
#include "CommonInclude.h"
#include "DefineEnum.h"

#include "BaseObject.h"
#include "PlayerObject.h"

#include "PacketDefine.h"
#include "MakePacket.h"
#include "Network.h"

#include "SpriteDib.h"


extern CSpriteDib g_cSprite;




////////////////////////////////////////////////////////////////////
// 생성자, 파괴자.
//
////////////////////////////////////////////////////////////////////
CPlayerObject::CPlayerObject(int iObjectType, int iObjectID, BOOL bPlayerCharacter) : CBaseObject(iObjectType, iObjectID)
{
	m_dwActionCur = dfACTION_STAND;
	m_dwActionOld = dfACTION_STAND;

	m_iDirCur = dfDIR_RIGHT;
	m_iDirOld = dfDIR_RIGHT;

	m_chHP = 0;

	m_bPlayerCharacter = bPlayerCharacter;
}


CPlayerObject::~CPlayerObject()
{

}

// =================================================================
// 오브젝트 공통 인터페이스 가상함수.
//
// =================================================================
////////////////////////////////////////////////////////////////////
// 오브젝트 액션 가상함수.
//
// Parameters: (DWORD)Param.
// Return: (DWORD)Value.
////////////////////////////////////////////////////////////////////
DWORD CPlayerObject::Action(DWORD dwParam)
{
	//------------------------------------------------------------
	// 다음 프레임으로 에니메이션.
	//------------------------------------------------------------
	NextFrame();

	//------------------------------------------------------------
	// m_dwActionPram 에 따라서 동작을 설정한다.
	//------------------------------------------------------------
	ActionProc();


	return FALSE;
}

////////////////////////////////////////////////////////////////////
// 오브젝트 그리기 가상함수.
//
// Parameters: (BYTE *)DestPtr. (int)DestWidth. (int)DestHeight. (int)DestPitch.
// Return: (DWORD)Value.
////////////////////////////////////////////////////////////////////
DWORD CPlayerObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//------------------------------------------------------------
	// 그림자 표시.
	//------------------------------------------------------------
	g_cSprite.DrawSprite50(eSHADOW,
							GetCurX(),
							GetCurY(),
							bypDest,
							iDestWidth,
							iDestHeight,
							iDestPitch);


	//------------------------------------------------------------
	// 플레이어의 경우 자신을 약간 빨간색으로 출력한다.
	//------------------------------------------------------------
	if ( IsPlayer() )
	{
		g_cSprite.DrawSpriteRed(GetSprite(),
								GetCurX(),
								GetCurY(),
								bypDest,
								iDestWidth,
								iDestHeight,
								iDestPitch);
	}
	else
	{
		g_cSprite.DrawSprite(GetSprite(),
							GetCurX(),
							GetCurY(),
							bypDest,
							iDestWidth,
							iDestHeight,
							iDestPitch);
	}

	//------------------------------------------------------------
	// HP 내용 표시.
	//------------------------------------------------------------
	g_cSprite.DrawSprite(eGUAGE_HP,
						GetCurX() - 35,
						GetCurY() + 9,
						bypDest,
						iDestWidth,
						iDestHeight,
						iDestPitch,
						GetHP());

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// 오브젝트 액션 변경 함수. 
// 베이스 클래스의 m_dwActionParam 값에 따라 동작을 변경해준다.
//
// Parameters: 없음.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CPlayerObject::ActionProc(void)
{
	//------------------------------------------------------------
	// 몇몇 동작시(공격)의 경우 강제적으로 해당 동작 처리를 완료해아만 한다.
	//------------------------------------------------------------
	switch ( m_dwActionCur )
	{
	//------------------------------------------------------------
	// 공격동작 또는 데미지 동작은 에니메이션이 끝날때까지 강제적으로 에니메이션이
	// 되어야만 하며, 에니메이션이 끝난 후 기본동작으로 자동으로 돌아가야 한다.
	//------------------------------------------------------------
	case dfACTION_ATTACK1:
	case dfACTION_ATTACK2:
	case dfACTION_ATTACK3:

		if ( IsEndFrame() )
		{
			SetActionStand();
			//------------------------------------------------------------
			// 공격이 끝났었더라면, 액션을 바꿔줘서 연속으로 공격을 할때
			// 재 전송이 가능하도록 한다.
			//------------------------------------------------------------
			m_dwActionInput = dfACTION_STAND;

		}
		break;


	default:
		//------------------------------------------------------------
		// 이외의 경우에는 사용자 입력 처리를 해준다.
		//------------------------------------------------------------
		InputActionProc();
		break;
	}
}



////////////////////////////////////////////////////////////////////
// 사용자 입력 액션에 따라 동작을 변경해준다.
//
// Parameters: 없음.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CPlayerObject::InputActionProc(void)
{
	//------------------------------------------------------------
	// 이전 액션과, 현재 액션을 비교하여 어떤 동작이 새롭게
	// 변경되었는지 확인하여 패킷을 서버에게 보낸다.
	//------------------------------------------------------------
	m_dwActionOld = m_dwActionCur;



	//------------------------------------------------------------
	// 어떤 동작도 없을경우 기본동작 스탠드로 바꿔준다.
	//------------------------------------------------------------
	if ( dfACTION_STAND == m_dwActionInput )
	{
		SetActionStand();
	}


	//------------------------------------------------------------
	// 공격 입력이 있을 경우 공격 동작으로 바꿔준다.
	//------------------------------------------------------------
	if ( dfACTION_ATTACK1 == m_dwActionInput )
	{
		SetActionAttack1();
	}
	else if ( dfACTION_ATTACK2 == m_dwActionInput )
	{
		SetActionAttack2();
	}
	else if ( dfACTION_ATTACK3 == m_dwActionInput )
	{
		SetActionAttack3();
	}


	//------------------------------------------------------------
	// 8방향에 대한 체크를 해서 이동시키도록 한다.
	// 네트워크 사용시 8방향으로 이동해야 하기 때문에...
	//
	// 이동가능 영역 체크를 하여 처리한다.
	//------------------------------------------------------------
	//------------------------------------------------------------
	// 이동 영역 체크 및 이동 처리
	//------------------------------------------------------------
	switch ( m_dwActionInput )
	{
	case dfACTION_MOVE_LL:
		{
			if ( dfRANGE_MOVE_LEFT < m_iCurX - dfSPEED_PLAYER_X )
				SetPosition(m_iCurX - dfSPEED_PLAYER_X, m_iCurY);

			SetDirection(dfDIR_LEFT);
			SetActionMove(m_dwActionInput);
		}
		break;


	case dfACTION_MOVE_LU:
		{
			if ( dfRANGE_MOVE_TOP < m_iCurY - dfSPEED_PLAYER_Y &&
				 dfRANGE_MOVE_LEFT < m_iCurX - dfSPEED_PLAYER_X )
			 
			SetPosition(m_iCurX - dfSPEED_PLAYER_X, m_iCurY - dfSPEED_PLAYER_Y);

			SetDirection(dfDIR_LEFT);
			SetActionMove(m_dwActionInput);
		}
		break;


	case dfACTION_MOVE_UU:
		{
			if ( dfRANGE_MOVE_TOP < m_iCurY - dfSPEED_PLAYER_Y )
				SetPosition(m_iCurX, m_iCurY - dfSPEED_PLAYER_Y);
		
			SetActionMove(m_dwActionInput);
		}
		break;


	case dfACTION_MOVE_RU:
		{
			if ( dfRANGE_MOVE_TOP < m_iCurY - dfSPEED_PLAYER_Y &&
				 dfRANGE_MOVE_RIGHT > m_iCurX + dfSPEED_PLAYER_X )
				SetPosition(m_iCurX + dfSPEED_PLAYER_X, m_iCurY - dfSPEED_PLAYER_Y);

			SetDirection(dfDIR_RIGHT);
			SetActionMove(m_dwActionInput);
		}
		break;


	case dfACTION_MOVE_RR:
		{
			if ( dfRANGE_MOVE_RIGHT > m_iCurX + dfSPEED_PLAYER_X )
				SetPosition(m_iCurX + dfSPEED_PLAYER_X, m_iCurY);
	
			SetDirection(dfDIR_RIGHT);
			SetActionMove(m_dwActionInput);
		}
		break;


	case dfACTION_MOVE_RD:
		{
			if ( dfRANGE_MOVE_BOTTOM > m_iCurY + dfSPEED_PLAYER_Y &&
				 dfRANGE_MOVE_RIGHT > m_iCurX + dfSPEED_PLAYER_X )
				SetPosition(m_iCurX + dfSPEED_PLAYER_X, m_iCurY + dfSPEED_PLAYER_Y);

			SetDirection(dfDIR_RIGHT);
			SetActionMove(m_dwActionInput);
		}
		break;


	case dfACTION_MOVE_DD:	
		{
			if ( dfRANGE_MOVE_BOTTOM > m_iCurY + dfSPEED_PLAYER_Y )
				SetPosition(m_iCurX, m_iCurY + dfSPEED_PLAYER_Y);

			SetActionMove(m_dwActionInput);
		}
		break;


	case dfACTION_MOVE_LD:
		{
			if ( dfRANGE_MOVE_BOTTOM > m_iCurY + dfSPEED_PLAYER_Y &&
				 dfRANGE_MOVE_LEFT < m_iCurX - dfSPEED_PLAYER_X )
				SetPosition(m_iCurX - dfSPEED_PLAYER_X, m_iCurY + dfSPEED_PLAYER_Y);

			SetDirection(dfDIR_LEFT);
			SetActionMove(m_dwActionInput);
		}
		break;
	}







	//------------------------------------------------------------
	// 이전 프레임의 동작과, 현재 프레임의 동작을 비교하여
	// 어떤 동작의 변화가 있었는지 확인한 뒤, 변화된 내역을 패킷으로 보낸다.
	//
	// 단, 플레이어 캐릭터일 경우에만 상태 변화에 따른 패킷을 보내야 한다.
	//------------------------------------------------------------
	if ( m_dwActionCur == m_dwActionOld || !m_bPlayerCharacter )
		return;

	CAyaPacket	clPacket;

	//------------------------------------------------------------
	// 이동중에 아무런 동작도 입력이 없게 변경된거라면 
	// 이동멈춤 패킷을 보내야 한다.
	//------------------------------------------------------------
	if ( (dfACTION_MOVE_LL <= m_dwActionOld && dfACTION_MOVE_LD >= m_dwActionOld )
		 && dfACTION_STAND == m_dwActionCur )
	{
		mpMoveStop(&clPacket, GetDirection(), GetCurX(), GetCurY()); //Make Packet
	}
	else
	{
		//------------------------------------------------------------
		// 동작 정보에 따라 패킷을 만든다.
		//------------------------------------------------------------
		switch ( m_dwActionCur )
		{
		case dfACTION_ATTACK1:
			mpAttack1(&clPacket, GetDirection(), GetCurX(), GetCurY());
			break;

		case dfACTION_ATTACK2:
			mpAttack2(&clPacket, GetDirection(), GetCurX(), GetCurY());
			break;

		case dfACTION_ATTACK3:
			mpAttack3(&clPacket, GetDirection(), GetCurX(), GetCurY());
			break;

		case dfACTION_MOVE_UU:
		case dfACTION_MOVE_RU:
		case dfACTION_MOVE_RR:
		case dfACTION_MOVE_RD:
		case dfACTION_MOVE_DD:
		case dfACTION_MOVE_LD:
		case dfACTION_MOVE_LL:
		case dfACTION_MOVE_LU:
			mpMoveStart(&clPacket, (BYTE)m_dwActionCur, GetCurX(), GetCurY());
			break;
		}
	}	

	//------------------------------------------------------------
	// 패킷 전송!!
	//------------------------------------------------------------
	netSendPacket(&clPacket);

}

////////////////////////////////////////////////////////////////////
// HP 셋팅.
//
// Parameters: (char)HP.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CPlayerObject::SetHP(char chHP)
{
	m_chHP = chHP;
}

////////////////////////////////////////////////////////////////////
// HP 얻기
//
// Parameters: 없음.
// Return: (int)HP.
////////////////////////////////////////////////////////////////////
int CPlayerObject::GetHP(void)
{
	return m_chHP;
}



// =================================================================
// 동작 상태, 스프라이트 범위를 바꿔주는 함수.
// 본 함수들은 내부함수로, SetAction() 함수에서 호출된다.
// =================================================================
////////////////////////////////////////////////////////////////////
// 각각의 동작으로 동작타입과, 스프라이트를 변경해준다.
//
// Parameters: 없음.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CPlayerObject::SetActionStand(void)
{
	//------------------------------------------------------------
	// 원래 기본 동작이 아니었거나, 방향이 바뀐 경우라면
	// 기본 스프라이트로 새로 바꿔준다.
	//------------------------------------------------------------
	if ( dfACTION_STAND != m_dwActionCur || m_iDirOld != m_iDirCur )
	{
		SetDirection(m_iDirCur);
		//------------------------------------------------------------
		// 방향에 따라서 정지 동작 변경
		//------------------------------------------------------------
		if ( dfDIR_LEFT == GetDirection() )
		{
			SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, dfDELAY_STAND);
		}
		else if ( dfDIR_RIGHT == GetDirection() )
		{
			SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, dfDELAY_STAND);
		}
	}
	m_dwActionCur = dfACTION_STAND;
}



void CPlayerObject::SetActionMove(int iAction)
{
	//------------------------------------------------------------
	// 원래 걷기 동작이 아니었거나, 방향이 바뀐 경우라면
	// 이동 스프라이트로 새로 바꿔준다.
	//------------------------------------------------------------
	if ( !(dfACTION_MOVE_LL <= m_dwActionCur && dfACTION_MOVE_LD >= m_dwActionCur) || m_iDirOld != m_iDirCur )
	{
		SetDirection(m_iDirCur);

		if ( dfDIR_LEFT == GetDirection() )
		{
			SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, dfDELAY_MOVE);
		}
		else
		{
			SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, dfDELAY_MOVE);
		}
	}
	m_dwActionCur = iAction;
}



void CPlayerObject::SetActionAttack1(void)
{
	//------------------------------------------------------------
	// 공격 동작중이 아니었다면 바꿔준다. 공격중에 또 공격은 안되므로...
	//------------------------------------------------------------
	if ( dfACTION_ATTACK1 != m_dwActionCur )
	{
		if ( dfDIR_LEFT == GetDirection() )
		{
			SetSprite(ePLAYER_ATTACK1_L01, ePLAYER_ATTACK1_L_MAX, dfDELAY_ATTACK1);
		}
		else
		{
			SetSprite(ePLAYER_ATTACK1_R01, ePLAYER_ATTACK1_R_MAX, dfDELAY_ATTACK1);
		}
	}
	m_dwActionCur = dfACTION_ATTACK1;
}



void CPlayerObject::SetActionAttack2(void)
{
	//------------------------------------------------------------
	// 공격 동작중이 아니었다면 바꿔준다. 공격중에 또 공격은 안되므로...
	//------------------------------------------------------------
	if ( dfACTION_ATTACK2 != m_dwActionCur )
	{
		if ( dfDIR_LEFT == GetDirection() )
		{
			SetSprite(ePLAYER_ATTACK2_L01, ePLAYER_ATTACK2_L_MAX, dfDELAY_ATTACK2);
		}
		else
		{
			SetSprite(ePLAYER_ATTACK2_R01, ePLAYER_ATTACK2_R_MAX, dfDELAY_ATTACK2);
		}
	}

	m_dwActionCur = dfACTION_ATTACK2;

}

void CPlayerObject::SetActionAttack3(void)
{
	//------------------------------------------------------------
	// 공격 동작중이 아니었다면 바꿔준다. 공격중에 또 공격은 안되므로...
	//------------------------------------------------------------
	if ( dfACTION_ATTACK3 != m_dwActionCur )
	{
		if ( dfDIR_LEFT == GetDirection() )
		{
			SetSprite(ePLAYER_ATTACK3_L01, ePLAYER_ATTACK3_L_MAX, dfDELAY_ATTACK3);
		}
		else
		{
			SetSprite(ePLAYER_ATTACK3_R01, ePLAYER_ATTACK3_R_MAX, dfDELAY_ATTACK3);
		}
	}
	m_dwActionCur = dfACTION_ATTACK3;

}



////////////////////////////////////////////////////////////////////
// 오브젝트 방향 셋팅.
//
// Parameters: (int)Direction.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CPlayerObject::SetDirection(int iDir)
{
	m_iDirOld = m_iDirCur;
	m_iDirCur = iDir;
}

int CPlayerObject::GetDirection(void)
{
	return m_iDirCur;
}


////////////////////////////////////////////////////////////////////
// 플레이어 객체 확인.
//
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL CPlayerObject::IsPlayer(void)
{
	return m_bPlayerCharacter; 
}

