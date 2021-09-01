
/*---------------------------------------------------------------

모든 객체의 베이스 클래스로 써
화면상에 나오는 모든 객체들은 본 클래스를 상속받는다.

---------------------------------------------------------------*/
#include <windows.h>
#include "BaseObject.h"
#include "DefineEnum.h"


////////////////////////////////////////////////////////////////////
// 생성자, 파괴자.
//
////////////////////////////////////////////////////////////////////
CBaseObject::CBaseObject(int iObjectType, int iObjectID)
{
	m_iObjectType = iObjectType;
	m_iObjectID = iObjectID;

	m_iOldX = -1;
	m_iOldY = -1;
	m_iCurX = -1;
	m_iCurY = -1;

	m_iSpriteStart = -1;
	m_iSpriteNow = 0;
	m_iSpriteMax = 0;
	m_iFrameDelay = 0;
	m_iDelayCount = 0;

	m_dwActionInput = dfACTION_STAND;
	m_bEndFrame = FALSE;
}

CBaseObject::~CBaseObject()
{

}


// =================================================================
// 오브젝트 정보 관련 함수.
//
// =================================================================
////////////////////////////////////////////////////////////////////
// 현재 위치 얻기.
//
// Parameters: 없음.
// Return: (int)위치값.
////////////////////////////////////////////////////////////////////
int	CBaseObject::GetOldX(void)
{
	return m_iOldX;
}

int CBaseObject::GetOldY(void)
{
    return m_iOldY;
}

int CBaseObject::GetCurX(void)
{
	return m_iCurX;
}

int CBaseObject::GetCurY(void) const
{
	return m_iCurY;
}

////////////////////////////////////////////////////////////////////
// 현재 위치 설정.
//
// Parameters: (int)X. (int)Y.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CBaseObject::SetCurPosition(int iX, int iY)
{
	m_iCurX = iX;
	m_iCurY = iY;
}

void CBaseObject::SetOldPosition(int iX, int iY)
{
	m_iOldX = iX;
	m_iOldY = iY;
}

void CBaseObject::SetPosition(int iX, int iY)
{
	//------------------------------------------------------------------
	// 기존 좌표를 Old좌표로 저장하고, 새 좌표를 Cur좌표에 저장.
	//------------------------------------------------------------------
	m_iOldX = m_iCurX;
	m_iOldY = m_iCurY;

	m_iCurX = iX;
	m_iCurY = iY;
}


////////////////////////////////////////////////////////////////////
// 오브젝트 Type, ID 얻기.
//
// Parameters: 없음.
// Return: (int)Type or ID.
////////////////////////////////////////////////////////////////////
int CBaseObject::GetObjectType(void) const
{
	return m_iObjectType;
}

int CBaseObject::GetObjectID(void)
{
    return m_iObjectID;
}


////////////////////////////////////////////////////////////////////
// 오브젝트 Type, ID 설정.
//
// Parameters: (int)Type or ID.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CBaseObject::SetObjectType(int iType)
{
	m_iObjectType = iType;
}

void CBaseObject::SetObjectID(int iID)
{
	m_iObjectID = iID;
}

////////////////////////////////////////////////////////////////////
// 스프라이트 정보 입력.
//
// Parameters: (int)SpriteStart. (int)SpriteMax. (int)FrameDelay.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CBaseObject::SetSprite(int iSpriteStart, int iSpriteMax, int iFrameDelay)
{
	m_iSpriteStart = iSpriteStart;
	m_iSpriteMax = iSpriteMax;
	m_iFrameDelay = iFrameDelay;

	m_iSpriteNow = iSpriteStart;
	m_iDelayCount = 0;
	m_bEndFrame = FALSE;
}

////////////////////////////////////////////////////////////////////
// 스프라이트 정보 얻음
//
// Parameters: 없음.
// Return: (int)Sprite.
////////////////////////////////////////////////////////////////////
int CBaseObject::GetSprite(void)
{
	return m_iSpriteNow;
}


////////////////////////////////////////////////////////////////////
// 현재 스프라이트 에니메이션이 마지막 인지 확인
//
// Parameters: 없음.
// Return: (int)Sprite.
////////////////////////////////////////////////////////////////////
int CBaseObject::IsEndFrame(void)
{
	return m_bEndFrame;
}



////////////////////////////////////////////////////////////////////
// 다음 프레임으로 넘김.
//
// Parameters: 없음.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void CBaseObject::NextFrame(void)
{
	if ( 0 > m_iSpriteStart )
		return;

	//------------------------------------------------------------
	// 프레임 딜레이 값을 넘어야 다음 프레임으로 넘어간다.
	//------------------------------------------------------------
	m_iDelayCount++;

	if ( m_iDelayCount >= m_iFrameDelay )
	{
		m_iDelayCount = 0;

		//------------------------------------------------------------
		// 에니메이션 끝으로 이동되면 처음으로 돌린다.
		//------------------------------------------------------------
		m_iSpriteNow++;
		
		if ( m_iSpriteNow > m_iSpriteMax )
		{
			m_iSpriteNow = m_iSpriteStart;
			m_bEndFrame = TRUE;
		}
	}
}


////////////////////////////////////////////////////////////////////
// 액션 입력.
//
// Parameters: (DWORD)Action.
// Return: 없음.
////////////////////////////////////////////////////////////////////
void  CBaseObject::ActionInput(DWORD dwAction)
{
	m_dwActionInput = dwAction;
}
