#pragma once
#include "TBasisStd.h"
#include <process.h>
#include "TProtocol.h"
#include "TStreamPacket.h"
// 어떤 메세지인지 구분해야 한다. ( 필요성 )
// 프로토콜 생성
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
	int				m_iEvent; // 이벤트 배열의 인덱스
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
	int			m_iEventCount;// 이벤트 배열의 개수

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