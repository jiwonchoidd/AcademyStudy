#pragma once
#include "TNetUser.h"

struct TPacket
{
	TNetUser*  pUser;
	UPACKET packet;
};
class TNetwork
{
public:
	std::list<TNetUser>		m_UserList;
	SOCKET					m_Sock;
	std::vector<TPacket>	m_recvPacket;	
	CRITICAL_SECTION		m_cs;     // userlist
	HANDLE					m_hMutex; // packetlist
	static bool				g_bConnect;
public:
	bool	MakePacket(UPACKET& packet,
						char* msg, int iLen, uint16_t type);
	bool	AddUser(SOCKET sock, SOCKADDR_IN addr);
	bool	DelUser(TNetUser& user);
	void	AddPacket(TNetUser& user, UPACKET* packet);
	virtual bool	RecvData(TNetUser& user);
	virtual bool	SendData(TNetUser& user, UPACKET& msg);
	int		SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type);
public:
	virtual bool Frame() {	return true;};
	virtual bool Run();
	virtual bool RecvUserList();
	virtual bool Broadcastting();
	virtual bool InitNetwork(std::string ip, int iPort);
	virtual bool InitSocket(std::string ip, int iPort);
	virtual bool DeleteNetwork();
	virtual bool Accept();
	virtual bool Process();
	virtual void PacketProcess();
public:
	TNetwork();
	virtual ~TNetwork();
};

