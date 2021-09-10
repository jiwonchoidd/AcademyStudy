
/*---------------------------------------------------------------

CBaseObject �� ��ӹ��� �÷��̾� ��ü.


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
// ������, �ı���.
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
// ������Ʈ ���� �������̽� �����Լ�.
//
// =================================================================
////////////////////////////////////////////////////////////////////
// ������Ʈ �׼� �����Լ�.
//
// Parameters: (DWORD)Param.
// Return: (DWORD)Value.
////////////////////////////////////////////////////////////////////
DWORD CPlayerObject::Action(DWORD dwParam)
{
	//------------------------------------------------------------
	// ���� ���������� ���ϸ��̼�.
	//------------------------------------------------------------
	NextFrame();

	//------------------------------------------------------------
	// m_dwActionPram �� ���� ������ �����Ѵ�.
	//------------------------------------------------------------
	ActionProc();


	return FALSE;
}

////////////////////////////////////////////////////////////////////
// ������Ʈ �׸��� �����Լ�.
//
// Parameters: (BYTE *)DestPtr. (int)DestWidth. (int)DestHeight. (int)DestPitch.
// Return: (DWORD)Value.
////////////////////////////////////////////////////////////////////
DWORD CPlayerObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//------------------------------------------------------------
	// �׸��� ǥ��.
	//------------------------------------------------------------
	g_cSprite.DrawSprite50(eSHADOW,
							GetCurX(),
							GetCurY(),
							bypDest,
							iDestWidth,
							iDestHeight,
							iDestPitch);


	//------------------------------------------------------------
	// �÷��̾��� ��� �ڽ��� �ణ ���������� ����Ѵ�.
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
	// HP ���� ǥ��.
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
// ������Ʈ �׼� ���� �Լ�. 
// ���̽� Ŭ������ m_dwActionParam ���� ���� ������ �������ش�.
//
// Parameters: ����.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayerObject::ActionProc(void)
{
	//------------------------------------------------------------
	// ��� ���۽�(����)�� ��� ���������� �ش� ���� ó���� �Ϸ��ؾƸ� �Ѵ�.
	//------------------------------------------------------------
	switch ( m_dwActionCur )
	{
	//------------------------------------------------------------
	// ���ݵ��� �Ǵ� ������ ������ ���ϸ��̼��� ���������� ���������� ���ϸ��̼���
	// �Ǿ�߸� �ϸ�, ���ϸ��̼��� ���� �� �⺻�������� �ڵ����� ���ư��� �Ѵ�.
	//------------------------------------------------------------
	case dfACTION_ATTACK1:
	case dfACTION_ATTACK2:
	case dfACTION_ATTACK3:

		if ( IsEndFrame() )
		{
			SetActionStand();
			//------------------------------------------------------------
			// ������ �����������, �׼��� �ٲ��༭ �������� ������ �Ҷ�
			// �� ������ �����ϵ��� �Ѵ�.
			//------------------------------------------------------------
			m_dwActionInput = dfACTION_STAND;

		}
		break;


	default:
		//------------------------------------------------------------
		// �̿��� ��쿡�� ����� �Է� ó���� ���ش�.
		//------------------------------------------------------------
		InputActionProc();
		break;
	}
}



////////////////////////////////////////////////////////////////////
// ����� �Է� �׼ǿ� ���� ������ �������ش�.
//
// Parameters: ����.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayerObject::InputActionProc(void)
{
	//------------------------------------------------------------
	// ���� �׼ǰ�, ���� �׼��� ���Ͽ� � ������ ���Ӱ�
	// ����Ǿ����� Ȯ���Ͽ� ��Ŷ�� �������� ������.
	//------------------------------------------------------------
	m_dwActionOld = m_dwActionCur;



	//------------------------------------------------------------
	// � ���۵� ������� �⺻���� ���ĵ�� �ٲ��ش�.
	//------------------------------------------------------------
	if ( dfACTION_STAND == m_dwActionInput )
	{
		SetActionStand();
	}


	//------------------------------------------------------------
	// ���� �Է��� ���� ��� ���� �������� �ٲ��ش�.
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
	// 8���⿡ ���� üũ�� �ؼ� �̵���Ű���� �Ѵ�.
	// ��Ʈ��ũ ���� 8�������� �̵��ؾ� �ϱ� ������...
	//
	// �̵����� ���� üũ�� �Ͽ� ó���Ѵ�.
	//------------------------------------------------------------
	//------------------------------------------------------------
	// �̵� ���� üũ �� �̵� ó��
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
	// ���� �������� ���۰�, ���� �������� ������ ���Ͽ�
	// � ������ ��ȭ�� �־����� Ȯ���� ��, ��ȭ�� ������ ��Ŷ���� ������.
	//
	// ��, �÷��̾� ĳ������ ��쿡�� ���� ��ȭ�� ���� ��Ŷ�� ������ �Ѵ�.
	//------------------------------------------------------------
	if ( m_dwActionCur == m_dwActionOld || !m_bPlayerCharacter )
		return;

	CAyaPacket	clPacket;

	//------------------------------------------------------------
	// �̵��߿� �ƹ��� ���۵� �Է��� ���� ����ȰŶ�� 
	// �̵����� ��Ŷ�� ������ �Ѵ�.
	//------------------------------------------------------------
	if ( (dfACTION_MOVE_LL <= m_dwActionOld && dfACTION_MOVE_LD >= m_dwActionOld )
		 && dfACTION_STAND == m_dwActionCur )
	{
		mpMoveStop(&clPacket, GetDirection(), GetCurX(), GetCurY()); //Make Packet
	}
	else
	{
		//------------------------------------------------------------
		// ���� ������ ���� ��Ŷ�� �����.
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
	// ��Ŷ ����!!
	//------------------------------------------------------------
	netSendPacket(&clPacket);

}

////////////////////////////////////////////////////////////////////
// HP ����.
//
// Parameters: (char)HP.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayerObject::SetHP(char chHP)
{
	m_chHP = chHP;
}

////////////////////////////////////////////////////////////////////
// HP ���
//
// Parameters: ����.
// Return: (int)HP.
////////////////////////////////////////////////////////////////////
int CPlayerObject::GetHP(void)
{
	return m_chHP;
}



// =================================================================
// ���� ����, ��������Ʈ ������ �ٲ��ִ� �Լ�.
// �� �Լ����� �����Լ���, SetAction() �Լ����� ȣ��ȴ�.
// =================================================================
////////////////////////////////////////////////////////////////////
// ������ �������� ����Ÿ�԰�, ��������Ʈ�� �������ش�.
//
// Parameters: ����.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CPlayerObject::SetActionStand(void)
{
	//------------------------------------------------------------
	// ���� �⺻ ������ �ƴϾ��ų�, ������ �ٲ� �����
	// �⺻ ��������Ʈ�� ���� �ٲ��ش�.
	//------------------------------------------------------------
	if ( dfACTION_STAND != m_dwActionCur || m_iDirOld != m_iDirCur )
	{
		SetDirection(m_iDirCur);
		//------------------------------------------------------------
		// ���⿡ ���� ���� ���� ����
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
	// ���� �ȱ� ������ �ƴϾ��ų�, ������ �ٲ� �����
	// �̵� ��������Ʈ�� ���� �ٲ��ش�.
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
	// ���� �������� �ƴϾ��ٸ� �ٲ��ش�. �����߿� �� ������ �ȵǹǷ�...
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
	// ���� �������� �ƴϾ��ٸ� �ٲ��ش�. �����߿� �� ������ �ȵǹǷ�...
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
	// ���� �������� �ƴϾ��ٸ� �ٲ��ش�. �����߿� �� ������ �ȵǹǷ�...
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
// ������Ʈ ���� ����.
//
// Parameters: (int)Direction.
// Return: ����.
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
// �÷��̾� ��ü Ȯ��.
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL CPlayerObject::IsPlayer(void)
{
	return m_bPlayerCharacter; 
}

