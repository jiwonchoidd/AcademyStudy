#pragma once
#include "TServerObj.h"
#include "TProtocol.h"
#include "TStreamPacket.h"
#include "TAcceptor.h"
#include "TDebugString.h"

class TCharacter : public TServerObj
{
public:
	int		m_iID;
	tGUID	m_Guid;
	float   m_fPos[3];
	float   m_fAngle[3];
	float   m_fVelocity[3];
	float   m_fHealth;
	bool    m_bAlive;
	bool    m_bAttacking;
public:
	friend ostream& operator<<(ostream &stream, TCharacter& tChar)
	{
		stream << tChar.m_iID << endl;
		stream << tChar.m_Guid << endl;
		stream << tChar.m_fPos[0] << endl;
		stream << tChar.m_fPos[1] << endl;
		stream << tChar.m_fPos[2] << endl;
		stream << tChar.m_fAngle[0] << endl;
		stream << tChar.m_fAngle[1] << endl;
		stream << tChar.m_fAngle[2] << endl;
		stream << tChar.m_fVelocity[0] << endl;
		stream << tChar.m_fVelocity[1] << endl;
		stream << tChar.m_fVelocity[2] << endl;		
		stream << tChar.m_fHealth << endl;
		stream << tChar.m_bAttacking << endl;
		stream << tChar.m_bAlive << endl;
		return stream;
	}

	friend istream& operator>>(istream& stream, TCharacter& tChar)
	{
		stream >> tChar.m_iID;
		stream >> tChar.m_Guid;
		stream >> tChar.m_fPos[0];
		stream >> tChar.m_fPos[1];
		stream >> tChar.m_fPos[2];
		stream >> tChar.m_fAngle[0];
		stream >> tChar.m_fAngle[1];
		stream >> tChar.m_fAngle[2];
		stream >> tChar.m_fVelocity[0];
		stream >> tChar.m_fVelocity[1];
		stream >> tChar.m_fVelocity[2];		
		stream >> tChar.m_fHealth;
		stream >> tChar.m_bAlive;
		stream >> tChar.m_bAttacking;
		return stream;
	}
public:
	TCharacter();
	virtual ~TCharacter();
};

