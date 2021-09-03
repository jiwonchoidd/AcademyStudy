#pragma once
#include "TObject.h"

class TGameUser: public TSingleton<TGameUser>
{
private:
	friend class TSingleton<TGameUser>;
public:
	int				m_iModelType; // A or B
	WORD			m_Direction;
	WORD			m_fPosX;
	WORD			m_fPosY;
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
	TGameUser(void);
	~TGameUser(void);
};
#define I_GameUser TGameUser::GetInstance()