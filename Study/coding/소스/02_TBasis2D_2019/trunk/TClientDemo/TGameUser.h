#pragma once
#include "TCharacter.h"

class TGameUser: public TSingleton<TGameUser>
{
private:
	friend class TSingleton<TGameUser>;
public:
	int				m_iGuid;
	tGUID			m_tGuid;
	int				m_iModelType; // A or B
	TCHAR			m_Name[20];
	int				m_iDirection;
	WORD			m_fPosX;
	WORD			m_fPosY;
	bool			m_IsSpawn;
	std::vector<TGameUser>	m_UserList;
	tGUID			m_GuidActiveCharacter;
	TCharacter*     m_pCharacter;
	TCharacterMap   m_CharacterList;
	FVector			GetActorLocation();
	FRotator		GetActorRotation();
	FVector			GetVelocity();
	void			MoveObject( int x, int y )
	{
		m_fPosX = x;
		m_fPosY = y;
	}
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	void		MoveLeft(int iUserID);
	void		MoveRight(int iUserID);
	void		MoveUp(int iUserID);
	void		MoveDown(int iUserID);
public:
	friend ostream& operator<<(ostream &stream, TGameUser& info)
	{
		tBlockWrite(stream, info.m_tGuid);
		tBlockWrite(stream, info.m_Name);
		tBlockWrite(stream, info.m_GuidActiveCharacter);
		stream << info.m_CharacterList;
		return stream;
	}

	friend istream& operator>>(istream& stream, TGameUser& info)
	{
		tBlockRead(stream, info.m_tGuid);
		tBlockRead(stream, info.m_Name);
		tBlockRead(stream, info.m_GuidActiveCharacter);
		stream >> info.m_CharacterList;
		return stream;
	}
public:
	TGameUser(void);
	virtual ~TGameUser(void);
};
#define I_GameUser TGameUser::GetInstance()