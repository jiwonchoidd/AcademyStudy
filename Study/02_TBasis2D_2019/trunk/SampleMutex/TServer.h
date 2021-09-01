#pragma once
#include "TBasisStd.h"
#include <process.h>
#include "TProtocol.h"
#include "TStreamPacket.h"
// � �޼������� �����ؾ� �Ѵ�. ( �ʿ伺 )
// �������� ����
struct TUser
{
	string		   m_Name;
	SOCKET		   m_Socket;
	TStreamPacket  m_StreamPacket;
};


class TServer: public TSingleton<TServer>
{
private:
	friend class TSingleton<TServer>;
public:
	TStreamPacket  m_StreamPacket;
	std::list<TUser>			m_UserList;
	typedef std::list<TUser>::iterator	m_ListItor;
	int			m_iClientNumber;
	HANDLE		m_Mutex;
	SOCKET		listen_sock;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	int			CreateListenSocket( int iPort);
	void		ProcessPacket();
public:
	TServer(void);
	~TServer(void);
};
#define I_Server TServer::GetInstance()