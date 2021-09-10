
/*---------------------------------------------------------------

CBaseObject �� ��ӹ��� ����Ʈ ��ü.


---------------------------------------------------------------*/
#include <windows.h>
#include "BaseObject.h"
#include "EffectObject.h"
#include "DefineEnum.h"
#include "SpriteDib.h"

extern CSpriteDib g_cSprite;


//////////////////////////////////////////////////////////////////////////
// SessionID�� ��ü �˻�.
//
// Parameters: (DWORD)dwSessionID.
// Return: (CBaseObject *)��ü ������.
//////////////////////////////////////////////////////////////////////////
CBaseObject *FindPlayerObject(DWORD dwSessionID);



////////////////////////////////////////////////////////////////////
// ������, �ı���.
//
////////////////////////////////////////////////////////////////////
CEffectObject::CEffectObject(int iObjectType, int iObjectID, DWORD dwAttackID) : CBaseObject(iObjectType, iObjectID)
{
	m_dwAttackID = dwAttackID;

	if ( 0xffffffff == dwAttackID )
        m_bEffectStart = TRUE;
	else
		m_bEffectStart = FALSE;

}

CEffectObject::~CEffectObject()
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
DWORD CEffectObject::Action(DWORD dwParam)
{
	//------------------------------------------------------------
	// �����ڸ� ã�Ƽ� �׼ǰ� �������� ���Ͽ� 
	// ������ ����Ʈ�� ������ Ÿ�̹��� ��´�.
	//------------------------------------------------------------
	if ( FALSE == m_bEffectStart )
	{
		CBaseObject *clpAttackObject = FindPlayerObject(m_dwAttackID);

		if ( NULL != clpAttackObject )
		{
			switch ( clpAttackObject->GetSprite() )
			{
			case ePLAYER_ATTACK1_L01:
			case ePLAYER_ATTACK1_R01:
			case ePLAYER_ATTACK2_L01:
			case ePLAYER_ATTACK2_R01:
			case ePLAYER_ATTACK3_L04:
			case ePLAYER_ATTACK3_R04:
				m_bEffectStart = TRUE;
				break;
			}
		}
		else
		{
			m_bEffectStart = TRUE;
		}
	}

	//------------------------------------------------------------
	// ���� ���������� ���ϸ��̼�.
	//------------------------------------------------------------
	if ( m_bEffectStart )
	{
		NextFrame();

		//------------------------------------------------------------
		// ������ �����ӱ��� ���ٸ� �ı�����.
		//------------------------------------------------------------
		if ( IsEndFrame() )
		{
			return TRUE;
		}
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////
// ������Ʈ �׸��� �����Լ�.
//
// Parameters: (BYTE *)DestPtr. (int)DestWidth. (int)DestHeight. (int)DestPitch.
// Return: (DWORD)Value.
////////////////////////////////////////////////////////////////////
DWORD CEffectObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	if ( m_bEffectStart )
	{
		g_cSprite.DrawSpriteRed(GetSprite(),
								GetCurX(),
								GetCurY(),
								bypDest,
								iDestWidth,
								iDestHeight,
								iDestPitch);
	}

	return FALSE;
}

