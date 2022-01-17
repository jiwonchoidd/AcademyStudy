#pragma once
#include "KPacket.h"
#include "KNetStd.h"
#include "KServerObj.h"
struct KOV 
{
	//��ܿ� ������ ĳ������ �����ϴ�.
	OVERLAPPED ov;
	int type;
};
//�ٸ� ������ �������� ����� ������ �ȵȴ�. 
//���� ������ ���������� ��ӹ޾Ƽ� ���
class KNetworkUser : public KServerObj
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	bool		m_bConnect = false;

	//������ ����ü
	KOV			m_RecvOV;
	KOV			m_SendOV;
	WSABUF		m_WsaRecvBuffer;
	WSABUF		m_WsaSendBuffer;
	char		m_szRecv[2048];
	char		m_szSend[2048];

	// buffer ����� �ʿ���
	char			m_szRecvBuffer[2048];
	int				m_iPacketPos; // ��Ŷ�� ���� �ּ�
	int				m_iWritePos; // ������ ���� ��ġ 
	int				m_iReadPos; // ��Ŷ ���� ��ġ�� ���� ���� ����Ʈ
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

