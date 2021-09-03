#pragma once
#include "TPlayerCharacter.h"

#define MAX_USER_RECV_SIZE 512

class TSession : public TServerObj
{
public:
	SOCKET			m_Socket;
	SOCKADDR_IN		m_addr;
public:
	TSession();
	virtual ~TSession();
};
class TBaseUser : public TSession
{
public:
	virtual void  Create() = 0;
	virtual void  Destory() = 0;
	virtual void  SetSocket(SOCKET sock, SOCKADDR_IN  client) = 0;
public:
	TBaseUser() {}
	virtual ~TBaseUser() {}
};

class TUser : public TBaseUser
{
public:
	RUNTIME_DECLARE(TUser)
private:	
	TCharacter*     m_pCharacter;
	TCharacterList  m_CharacterList;
	TServerObj		m_LockOverlapped;
public:
	TCharacterList& GetCharacterList();
	void			SetActiveCharacter(TCharacter*, bool bAlive);
public:
	OVERLAPPED2		m_Overlapped;
	OVERLAPPED2		m_OverlappedZero;
	OVERLAPPED2&    Overlapped(int iType);
	OVERLAPPED2&    OverlappedZero();
	int				m_iType;
	tGUID		    m_LoginGuid;
	tGUID		    m_tGuid;
	tGUID			m_GuidActiveCharacter;
	CONNECT_TYPE    m_ConnectType;
	string			m_Name;
	WSABUF			m_wsaRecvBuffer;
	WSABUF			m_wsaZeroBuffer;
	WSABUF			m_wsaSendBuffer;
	char			m_strBuffer[MAX_USER_RECV_SIZE];
	char			m_sendBuffer[MAX_USER_RECV_SIZE];
	TStreamPacket	m_StreamPacket;
	bool			m_bUsed;
public:
	int	    Dispatch(DWORD deByteSize, LPOVERLAPPED ov);
	int		WaitForPacketReceive();
	int		WaitForZeroByteReceive();
	int		WaitReceive();
	void	Release();
public:
	virtual void  Create() override {}
	virtual void  Destory() override {};
	virtual void SetSocket(SOCKET sock, SOCKADDR_IN addr) override
	{
		m_Socket = sock;
		m_addr = addr;
	};
public:
	friend ostream& operator<<(ostream &stream, TUser& info);
	friend istream& operator>>(istream& stream, TUser& info);
public:
	TUser();
	virtual ~TUser();
};
