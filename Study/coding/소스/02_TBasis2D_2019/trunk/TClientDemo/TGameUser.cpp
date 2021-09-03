#include "TGameUser.h"
#include "TDemoObjMgr.h"

FVector TGameUser::GetActorLocation()
{
	if (m_pCharacter != nullptr)
	{
		return m_pCharacter->GetActorLocation();
	}
	return FVector();
}
FRotator TGameUser::GetActorRotation()
{
	if (m_pCharacter != nullptr)
	{
		return m_pCharacter->GetActorRotation();
	}
	return FRotator();
}
FVector TGameUser::GetVelocity()
{
	if (m_pCharacter != nullptr)
	{
		return m_pCharacter->GetVelocity();
	}
	return FVector();
}

bool TGameUser::Init()
{
	m_iModelType = 0;
	m_fPosX = (rand()%500) + 100;
	m_fPosY = (rand()%300) + 100;
	m_IsSpawn = false;	
	return true;
}
bool TGameUser::Render()
{
	if (m_IsSpawn)
	{
		I_DemoObjMgr.m_ObjectList[m_iModelType].SetPos(m_fPosX, m_fPosY);
		I_DemoObjMgr.m_ObjectList[m_iModelType].Render();
	}
	return true;
}
bool TGameUser::Frame()
{
	if(m_iDirection & 8)
	{
		MoveLeft(0);
	}
	if (m_iDirection & 4)
	{
		MoveDown(0); 
	}
	if (m_iDirection & 2)
	{
		MoveRight(0);
	}
	if (m_iDirection & 1)
	{
		MoveUp(0);
	}
	I_DemoObjMgr.m_ObjectList[m_iModelType].SetPos( m_fPosX, m_fPosY );
	return true;
}
bool TGameUser::Release()
{
	return true;
}
void TGameUser::MoveLeft( int iUserID )
{	
	float fPos = m_fPosX;
	fPos -= 100.0f * g_fSecPerFrame;
	if( fPos < 0 ) fPos = 0;
	m_fPosX = fPos;
}
void TGameUser::MoveRight(int iUserID )
{	
	float fPos = m_fPosX;
	fPos += 100.0f * g_fSecPerFrame;
	if( fPos > 780 ) fPos = 780;
	m_fPosX = fPos;	
}
void TGameUser::MoveUp(int iUserID )
{
	float fPos = m_fPosY;
	fPos -= 100.0f * g_fSecPerFrame;
	if( fPos < 0 ) fPos = 0;
	m_fPosY = fPos;	
}
void TGameUser::MoveDown(int iUserID )
{
	float fPos = m_fPosY;
	fPos += 100.0f * g_fSecPerFrame;
	if( fPos > 590 ) fPos = 590;
	m_fPosY = fPos;	
}
TGameUser::TGameUser(void)
{
	m_iDirection = 0;
}

TGameUser::~TGameUser(void)
{
}
