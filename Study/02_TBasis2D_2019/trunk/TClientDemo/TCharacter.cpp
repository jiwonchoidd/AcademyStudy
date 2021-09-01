#include "TCharacter.h"

tGUID  TCharacter::GetGuid() const
{
	return m_Info.tGuid;
};
FVector TCharacter::GetActorLocation()
{
	return m_Info.vPosition;
};
FVector TCharacter::GetVelocity()
{
	return m_Info.vVelocity;
};
FRotator TCharacter::GetActorRotation()
{
	return m_Info.vRotation;
};
float	TCharacter::GetHealth()
{
	return m_Info.fHealth;
};
int     TCharacter::GetType()
{
	return m_Info.iType;
}
bool	TCharacter::GetAlive()
{
	return m_Info.bAlive;
}     // 리스폰 가능여부(중복체크용)
bool	TCharacter::GetAttack()
{
	return m_Info.bAttack;
}    // 공격 가능여부(중복체크용)
bool	TCharacter::GetDamage()
{
	return m_Info.bDamage;
}	// 데미지 가능여부(중복체크용)
TPACKET_CHARACTER_INFO& TCharacter::GetInfo()
{
	return m_Info;
}

void  TCharacter::SetGuid(tGUID tGuid)
{
	m_Info.tGuid = tGuid;
};
void TCharacter::SetPos(FVector pos)
{
	m_Info.vPosition = pos;
};
void TCharacter::SetVelocity(FVector velo)
{
	m_Info.vVelocity = velo;
};
void TCharacter::SetRotation(FRotator rot)
{
	m_Info.vRotation = rot;
};
void TCharacter::SetHealth(float fHealth)
{
	m_Info.fHealth = fHealth;
};
void     TCharacter::SetType(int iType)
{
	m_Info.iType = iType;
}
void	TCharacter::SetAlive(bool bAlive)
{
	m_Info.bAlive = bAlive;
}
void	TCharacter::SetAttack(bool bAttack)
{
	m_Info.bAttack = bAttack;
}
void	TCharacter::SetDamage(bool bDamage)
{
	m_Info.bDamage = bDamage;
}
void TCharacter::SetInfo(TPACKET_CHARACTER_INFO info)
{
	m_Info = info;
}
void	TCharacter::Set(FVector vPosition, FRotator vRotation, FVector vVelocity)
{
	SetPos(vPosition);
	SetRotation(vRotation);
	SetVelocity(vVelocity);
}
void	TCharacter::Set()
{
	ZeroMemory(&m_Info, sizeof(TPACKET_CHARACTER_INFO));
	m_Info.fHealth = 1.0f;
	m_Info.bAlive = true;
	m_Info.bAttack = true;
	m_Info.bDamage = true;
}
TCharacter::TCharacter()
{
	Set();
}


TCharacter::~TCharacter()
{
}

ostream& operator<<(ostream &stream, TCharacter& tChar)
{
	stream << tChar.m_Info;
	return stream;
}
istream& operator>>(istream& stream, TCharacter& tChar)
{
	stream >> tChar.m_Info;
	return stream;
}
ostream& operator<<(ostream &stream, TCharacterMap& info)
{
	stream << info.tCharMap.size();
	for (auto& data : info.tCharMap)
	{
		stream << data.first;
		stream << data.second;
	}
	return stream;
}

istream &operator>>(istream &stream, TCharacterMap& info)
{
	info.tCharMap.clear();
	size_t m_iNumPlayers = 0;
	tGUID tGuid;
	TCharacter tChar;

	stream >> m_iNumPlayers;
	for (int i = 0; i < m_iNumPlayers; i++)
	{
		stream >> tGuid;
		stream >> tChar;
		info.tCharMap[tGuid] = tChar;
	}
	return stream;
}