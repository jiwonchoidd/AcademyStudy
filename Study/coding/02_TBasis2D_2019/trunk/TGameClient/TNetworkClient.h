#pragma once
#include "TNetwork.h"
class TNetworkClient : public TNetwork
{
public:
	HANDLE		g_hSendThread;
	std::vector<UPACKET>	m_sendPacket;
public:
	virtual bool  InitNetwork(std::string ip, int iPort) override;
	virtual bool  InitSocket(std::string ip, int port) override;
	virtual bool  RecvData(TNetUser& user) override;
	virtual void  PacketProcess() override;
public:
	void SendLoginData(SOCKET sock, std::string id, std::string ps);
public:
	TNetworkClient();
	virtual ~TNetworkClient();
};

