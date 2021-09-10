
/*---------------------------------------------------------------

��� ��ü�� ���̽� Ŭ������ ��
ȭ��� ������ ��� ��ü���� �� Ŭ������ ��ӹ޴´�.

---------------------------------------------------------------*/
#include <windows.h>
#include "BaseObject.h"
#include "DefineEnum.h"


////////////////////////////////////////////////////////////////////
// ������, �ı���.
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
// ������Ʈ ���� ���� �Լ�.
//
// =================================================================
////////////////////////////////////////////////////////////////////
// ���� ��ġ ���.
//
// Parameters: ����.
// Return: (int)��ġ��.
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
// ���� ��ġ ����.
//
// Parameters: (int)X. (int)Y.
// Return: ����.
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
	// ���� ��ǥ�� Old��ǥ�� �����ϰ�, �� ��ǥ�� Cur��ǥ�� ����.
	//------------------------------------------------------------------
	m_iOldX = m_iCurX;
	m_iOldY = m_iCurY;

	m_iCurX = iX;
	m_iCurY = iY;
}


////////////////////////////////////////////////////////////////////
// ������Ʈ Type, ID ���.
//
// Parameters: ����.
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
// ������Ʈ Type, ID ����.
//
// Parameters: (int)Type or ID.
// Return: ����.
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
// ��������Ʈ ���� �Է�.
//
// Parameters: (int)SpriteStart. (int)SpriteMax. (int)FrameDelay.
// Return: ����.
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
// ��������Ʈ ���� ����
//
// Parameters: ����.
// Return: (int)Sprite.
////////////////////////////////////////////////////////////////////
int CBaseObject::GetSprite(void)
{
	return m_iSpriteNow;
}


////////////////////////////////////////////////////////////////////
// ���� ��������Ʈ ���ϸ��̼��� ������ ���� Ȯ��
//
// Parameters: ����.
// Return: (int)Sprite.
////////////////////////////////////////////////////////////////////
int CBaseObject::IsEndFrame(void)
{
	return m_bEndFrame;
}



////////////////////////////////////////////////////////////////////
// ���� ���������� �ѱ�.
//
// Parameters: ����.
// Return: ����.
////////////////////////////////////////////////////////////////////
void CBaseObject::NextFrame(void)
{
	if ( 0 > m_iSpriteStart )
		return;

	//------------------------------------------------------------
	// ������ ������ ���� �Ѿ�� ���� ���������� �Ѿ��.
	//------------------------------------------------------------
	m_iDelayCount++;

	if ( m_iDelayCount >= m_iFrameDelay )
	{
		m_iDelayCount = 0;

		//------------------------------------------------------------
		// ���ϸ��̼� ������ �̵��Ǹ� ó������ ������.
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
// �׼� �Է�.
//
// Parameters: (DWORD)Action.
// Return: ����.
////////////////////////////////////////////////////////////////////
void  CBaseObject::ActionInput(DWORD dwAction)
{
	m_dwActionInput = dwAction;
}
