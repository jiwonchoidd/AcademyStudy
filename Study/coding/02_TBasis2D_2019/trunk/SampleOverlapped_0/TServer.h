#pragma once
#include "TBasisStd.h"
#include <process.h>
#include "TProtocol.h"
#include "TStreamPacket.h"
// � �޼������� �����ؾ� �Ѵ�. ( �ʿ伺 )
// �������� ����
typedef struct _OVERLAPPED2 : OVERLAPPED
{
	enum {MODE_RECV=0, MODE_SEND=1};
	int		m_iFlags;
	_OVERLAPPED2()
	{
		m_iFlags = MODE_RECV;
	}
	
}OVERLAPPED2;

struct TUser
{
	OVERLAPPED2		m_ov;
	int				m_iEvent; // �̺�Ʈ �迭�� �ε���
	string			m_Name;
	SOCKET			m_Socket;	
	WSABUF			m_wsaBuffer;
	char			m_strBuffer[2048];
	TStreamPacket	m_StreamPacket;
	bool			m_bUsed;
};

class TServer: public TSingleton<TServer>
{
private:
	friend class TSingleton<TServer>;
public:
	WSAEVENT			m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
public:
	TStreamPacket		m_StreamPacket;	
	std::list<TUser*>	m_UserList;
	typedef std::list<TUser*>::iterator	m_ListItor;
	int			m_iClientNumber;
	HANDLE		m_Mutex;
	SOCKET		m_ListenSock;
	int			m_iEventCount;// �̺�Ʈ �迭�� ����

	WSABUF		m_wsaBuffer;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	int			CreateListenSocket( int iPort);
	void		ProcessPacket();
public:
	TUser*		GetUser( int iIndex );
	bool		DelUser( int iIndex );
public:
	TServer(void);
	~TServer(void);
};
#define I_Server TServer::GetInstance()