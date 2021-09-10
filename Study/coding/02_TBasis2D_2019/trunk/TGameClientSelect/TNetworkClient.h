#pragma once
#include "TNetwork.h"
class TNetworkClient : public TNetwork
{
public:
	TNetUser				m_User;
	std::vector<UPACKET>	m_sendPacket;
	bool					m_bLogin;
	bool					m_bLobbyEntry;
	bool					m_bZoneEntry;
public:
	virtual bool  InitNetwork(std::string ip, int iPort) override;
	virtual bool  InitSocket(std::string ip, int port) override;
	virtual bool  RecvData(TNetUser& user) override;
	virtual void  PacketProcess() override;	
	LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	bool SendPackets(TNetUser& user);
	bool SendData(TNetUser& user, UPACKET& msg);
public:
	void AddSendPacket(UPACKET& packet);
public:
	TNetworkClient();
	virtual ~TNetworkClient();
};

class TNetworkClientSelect : public TNetworkClient
{
public:
	FD_SET		m_rSet;
	FD_SET		m_wSet;
	FD_SET		m_eSet;
public:
	virtual bool  Frame() override;
	virtual bool  InitNetwork(std::string ip, int iPort) override;
	virtual bool  InitSocket(std::string ip, int port) override;
public:
	TNetworkClientSelect() {};
	virtual ~TNetworkClientSelect() {};
};

class TNetworkClientEventSelect : public TNetworkClient
{
	HANDLE		m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
	int			m_iArrayCount;
public:
	virtual bool  Frame() override;
	virtual bool  InitNetwork(std::string ip, int iPort) override;
	virtual bool  InitSocket(std::string ip, int port) override;
public:
	TNetworkClientEventSelect() { m_iArrayCount = 0; };
	virtual ~TNetworkClientEventSelect() {};
};
