#include "TGamePlayer.h"
#include "TInput.h"
#include "TObjectMgr.h"
#include <time.h>
TObject* TGamePlayer::Clone()
{
	return new TGamePlayer(*this);
}
bool TGamePlayer::MakePacket(UPACKET& packet,char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}

bool TGamePlayer::Frame()
{
	m_dwMove = 0;
	TPoint dir(0, 0);
	if (g_bActive)
	{
		if (g_KeyMap.m_bRight)
		{
			dir.x = 1.0f;
			m_dwMove |= 8;
		}
		if (g_KeyMap.m_bLeft)
		{
			dir.x = -1.0f;
			m_dwMove |= 4;
		}
		if (g_KeyMap.m_bFront)
		{
			dir.y = -1.0f;
			m_dwMove |= 2;
		}
		if (g_KeyMap.m_bBack)
		{
			dir.y = 1.0f;
			m_dwMove |= 1;
		}
		m_ptDirection = dir.Normalize();		
		m_ptPos.x += m_tSendDirection.x * m_fSpeed * g_fSecondPerFrame;
		m_ptPos.y += m_tSendDirection.y * m_fSpeed * g_fSecondPerFrame;		
	}else
	{
		m_tSendDirection = m_ptDirection = dir.Normalize();	
		m_dwMove = m_dwPreMove = 0;
	}
	SetPos(m_ptPos);
	ProjectileTick();
	return true;
}
bool TGamePlayer::Damage()
{
	if (--m_iLifeCounter <= 0)
	{
		return Dead();
	}
	return false;
}
bool TGamePlayer::Dead()
{
	return true;
}

bool TGamePlayer::Hit(TPoint hitpos,int iNpcIndex)
{
	UPACKET sendPacket;
	TEnemyPos data;
	data.index = iNpcIndex;
	data.p = hitpos.get();
	data.d = m_ptDirection.get();
	data.t = data.p + data.d * m_fSpeed * 100.0f;
	if (data.d == TPoint::zero)
	{
		data.t = data.p;
	}
	MakePacket(sendPacket, (char*)&data,sizeof(TEnemyPos),PACKET_HIT_NPC);
	m_SendList.push_back(sendPacket);
	//m_Network.AddSendPacket(sendPacket);
	return false;
}
TGamePlayer::TGamePlayer()
{
	m_dwMove = 0;
	m_dwPreMove=0;
	m_iLifeCounter = 3;	
	m_tSendDirection.x = 0;
	m_tSendDirection.y = 0;
	m_fSpeed = 300.0f;
	m_fSendKeyInterval = 0.11f;
}
TGamePlayer::~TGamePlayer()
{
}



bool TGameOtherPlayer::Frame()
{	// 목표 위치 도착여부 판단->프레임->디렉션 = 0;
	float fDistance = (m_tTargetPos - m_ptPos).Length();
	if (fDistance <= 10.0f)
	{
		m_ptDirection = { 0, 0 };
		m_tTargetPos = m_ptPos;
	}
	m_ptPos.x += m_ptDirection.x * m_fSpeed * g_fSecondPerFrame;
	m_ptPos.y += m_ptDirection.y * m_fSpeed * g_fSecondPerFrame;
	SetPos(m_ptPos);
	// TODO : OtherUser Projectile frame
	ProjectileTick();
	return true;
}