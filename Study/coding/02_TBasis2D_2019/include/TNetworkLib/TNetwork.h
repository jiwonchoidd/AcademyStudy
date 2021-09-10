#pragma once
#include "TSelectModel.h"
class TNetwork
{
public:
	std::shared_ptr<TSelectModel> m_pModel;
	std::string         m_ipAddress;
	int					m_iPort;
	HWND				m_hWnd;
	SOCKET				m_Sock;
	static std::list<UPACKET>  m_RecvPool;
	static std::list<UPACKET>  m_SendPool;
	static bool			m_bConnect;
public:
	bool CreateSock();
	bool SendPacketPool();
	bool SendMsg(int type, char* data, int iSize);
	bool	Connect(const char* ip, int port, HWND hWnd);
	bool	Server(const char* ip=nullptr, int port=10000, HWND hWnd=NULL);
	LRESULT	MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool	ShutDown();
	bool	SetModel(std::shared_ptr<TSelectModel> pModel);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	TNetwork();
	virtual ~TNetwork();
};

