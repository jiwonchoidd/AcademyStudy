
/*---------------------------------------------------------------

CBaseObject 를 상속받은 이펙트 객체.


---------------------------------------------------------------*/
#include <windows.h>
#include "BaseObject.h"
#include "EffectObject.h"
#include "DefineEnum.h"
#include "SpriteDib.h"

extern CSpriteDib g_cSprite;


//////////////////////////////////////////////////////////////////////////
// SessionID로 객체 검색.
//
// Parameters: (DWORD)dwSessionID.
// Return: (CBaseObject *)객체 포인터.
//////////////////////////////////////////////////////////////////////////
CBaseObject *FindPlayerObject(DWORD dwSessionID);



////////////////////////////////////////////////////////////////////
// 생성자, 파괴자.
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
// 오브젝트 공통 인터페이스 가상함수.
//
// =================================================================
////////////////////////////////////////////////////////////////////
// 오브젝트 액션 가상함수.
//
// Parameters: (DWORD)Param.
// Return: (DWORD)Value.
////////////////////////////////////////////////////////////////////
DWORD CEffectObject::Action(DWORD dwParam)
{
	//------------------------------------------------------------
	// 공격자를 찾아서 액션과 프레임을 비교하여 
	// 동작이 이펙트를 시작할 타이밍을 잡는다.
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
	// 다음 프레임으로 에니메이션.
	//------------------------------------------------------------
	if ( m_bEffectStart )
	{
		NextFrame();

		//------------------------------------------------------------
		// 마지막 프레임까지 갔다면 파괴하자.
		//------------------------------------------------------------
		if ( IsEndFrame() )
		{
			return TRUE;
		}
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////
// 오브젝트 그리기 가상함수.
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

