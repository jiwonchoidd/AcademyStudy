#pragma once
#include "TGameUser.h"
#include "TProtocol.h"
class TGamePlayer : public TGameUser
{
	int		m_iLifeCounter;
public:
	std::vector<UPACKET>	m_SendList;
	TPoint					m_tSendDirection;
	float					m_fSendKeyInterval;
public:
	virtual TObject* Clone() override;
	virtual bool Frame() override;
	virtual bool Damage() override;
	virtual bool Dead() override;
	virtual bool Hit(TPoint p, int iNpcIndex);
	bool MakePacket(UPACKET& packet,char* msg, int iLen, uint16_t type);
public:
	TGamePlayer();
	virtual ~TGamePlayer();
};

class TGameOtherPlayer : public TGameUser
{
public:
	TPACKET_CHARACTER_INFO m_Info;
	virtual bool  Frame() override;
public:
	TGameOtherPlayer() {};
	virtual ~TGameOtherPlayer() {};
};
