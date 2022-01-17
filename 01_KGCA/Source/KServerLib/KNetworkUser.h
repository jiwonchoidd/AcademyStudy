#pragma once
#include "KPacket.h"
#include "KNetStd.h"
#include "KServerObj.h"
struct KOV 
{
	//상단에 있으면 캐스팅이 가능하다.
	OVERLAPPED ov;
	int type;
};
//다른 서버에 종속적인 기능이 있으면 안된다. 
//게임 유저면 게임유저로 상속받아서 사용
class KNetworkUser : public KServerObj
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	bool		m_bConnect = false;

	//오버랩 구조체
	KOV			m_RecvOV;
	KOV			m_SendOV;
	WSABUF		m_WsaRecvBuffer;
	WSABUF		m_WsaSendBuffer;
	char		m_szRecv[2048];
	char		m_szSend[2048];

	// buffer 기능이 필요함
	char			m_szRecvBuffer[2048];
	int				m_iPacketPos; // 패킷의 시작 주소
	int				m_iWritePos; // 현재의 저장 위치 
	int				m_iReadPos; // 패킷 시작 위치로 부터 받은 바이트
	std::list<KPacket> m_lPacketPool;
public:
	int		DispatchRead(char* sRecvBuffer, int iRecvByte);
	int		DispatchRecv(char* sRecvBuffer, int iRecvByte);
	int		DispatchSend(DWORD dwTransfer);
	int		Dispatch(DWORD dwTransfer, KOV* tov);
	void	Set(SOCKET sock, SOCKADDR_IN addr);
	int		Recv();
	bool	Disconnect();
};

