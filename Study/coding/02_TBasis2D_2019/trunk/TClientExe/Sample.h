//http://msdn.microsoft.com/ko-kr/library/35fhdx10.aspx
#pragma once
#include "TCore.h"
#include "TObject.h"
#include "TClient.h"
#include "TGameUser.h"
#include "TUdpSocket.h"

class Sample  : public TCore
{
public:	
	TUdpSocket			m_Udp;
	//게임유저
	//TGameUser				I_GameUser;
	std::vector<TGameUser>	m_UserList;
	TClient				m_Client;
	bool				m_bLogin;
	int					m_iSerIndex;
	POINT				m_CursorPos;
	int					m_iPlayState;
	int					m_iValueW;
	int					m_iValueS;
	int					m_iValueD;
	int					m_iValueA;
public:
	bool				Init();
	bool				PreRender();
	bool				Render();
	bool				PostRender();
	bool				Frame();
	bool				Release();	
public:
	Sample(void);
	~Sample(void);
};
