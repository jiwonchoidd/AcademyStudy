#pragma once
#include "TServer.h"
#include "TChatPacketPool.h"
#include "TChatAccept.h"
#include "TChatUser.h"

class TChatSvr : public TSingleton<TChatSvr>, public TServer
{
private:
	friend class TSingleton<TChatSvr>;
public:
	TChatPacketPool   m_PacketPool;	
	TChatAccept       m_Acceptor;
	
	//////////////  ���� ����Ʈ ///////////////////
	std::list<TChatUser*>	m_UserList;
	typedef std::list<TChatUser*>::iterator m_ListItor;
	int					m_iUserCount;
public:
	bool	Init();
	bool	Release();
	bool	Run();
	LRESULT MsgProc(HWND hWnd, 
						 UINT message, 
						 WPARAM wParam, 
						 LPARAM lParam);
	void	Print( char* fmt, ... );
	bool	AddUser(SOCKET socket, SOCKADDR_IN address);
	void	DeleteUser( SOCKET socket );
	void	Broadcastting( char* msg, WORD code );
	void	Broadcastting( T_PACKET* pUserData );
	void	Broadcastting( UPACKET* pPacket );

public:
	TChatSvr(void);
	~TChatSvr(void);
};
#define I_Server TChatSvr::GetInstance()

