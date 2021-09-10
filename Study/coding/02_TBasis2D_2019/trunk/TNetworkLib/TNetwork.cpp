#include "TNetwork.h"
#include "TAsyncSelect.h"
#include "TEventSelect.h"
#include "TStdSelect.h"

std::list<UPACKET>  TNetwork::m_RecvPool;
std::list<UPACKET>  TNetwork::m_SendPool;
bool				TNetwork::m_bConnect =false;

bool TNetwork::SendPacketPool()
{
	if (TNetwork::m_bConnect == false) return true;
	for (UPACKET& packet : TNetwork::m_SendPool)
	{
		if (m_pModel->SendMsg(packet) == false)
		{
			ShutDown();
			break;
		}
	}
	TNetwork::m_SendPool.clear();
	return true;
}
bool   TNetwork::SendMsg(int type, char* data, int iSize)
{
	UPACKET packet;
	ZeroMemory(&packet, sizeof(UPACKET));
	packet.ph.type = type;
	packet.ph.len = PACKET_HEADER_SIZE + iSize;
	if (data != nullptr)
	{
		memcpy(packet.msg, data, iSize);
	}
	m_SendPool.push_back(packet);
	return true;
}

bool	TNetwork::Connect(const char* ip, int port, HWND hWnd)
{
	m_ipAddress = ip;
	m_iPort = port;	
	m_hWnd = hWnd;

	if (CreateSock() == false)
	{
		return false;
	}

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);
	int ret = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		E_MSG("connect");
		return false;
	}
	m_bConnect = true;	
	return true;
}
bool	TNetwork::Server(const char* ip, int port, HWND hWnd)
{
	if (ip != nullptr)
	{
		m_ipAddress = ip;
	}
	m_iPort = port;
	m_hWnd = hWnd;

	if (CreateSock() == false)
	{
		return false;
	}

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if(ip == nullptr)
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		sa.sin_addr.s_addr = inet_addr(ip);
	int ret = bind(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		E_MSG("bind");
		return false;
	}
	ret = listen(m_Sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		E_MSG("listen");
		return false;
	}
	m_bConnect = true;
	return true;
}
bool	TNetwork::Init()
{	
	std::cout.sync_with_stdio(true);
	return true;
}
bool	TNetwork::CreateSock()
{
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET)
	{
		return false;
	}
	unsigned long iMode = 0;
	ioctlsocket(m_Sock, FIONBIO, &iMode);
	return true;
}
LRESULT	TNetwork::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pModel == nullptr) return 1;
	return m_pModel->MsgProc(hWnd, msg, wParam, lParam);
}
bool	TNetwork::SetModel(std::shared_ptr<TSelectModel> pModel)
{		
	if (pModel == nullptr) return false;
	///*m_pModel = new TAsyncSelect(m_Sock);
	//m_pModel->m_hWnd = hWnd;
	//m_pModel->Init();*/
	//m_pModel = new TEventSelect(m_Sock);
	//m_pModel->Init();
	//m_pModel = new TStdSelect(m_Sock);
	//m_pModel->Init();
	m_pModel = pModel;
	m_pModel->m_hWnd = m_hWnd;
	m_pModel->Init();
	return true;
}
bool	TNetwork::Frame() 
{
	if (m_pModel == nullptr) return true;	
	if (m_bConnect == false)
	{		
		// Àç Á¢¼Ó
		if (Connect(m_ipAddress.c_str(), m_iPort, m_hWnd)==false)
		{
			ShutDown();
			return false;
		}
		m_pModel->m_Sock = m_Sock;
		m_pModel->Init();
	}			
	if (m_pModel->Frame()==false)
	{
		ShutDown();
		return false;
	}
	return true;
}
bool	TNetwork::Render() {
	return true;
}
bool	TNetwork::ShutDown()
{
	m_bConnect = false;
	closesocket(m_Sock);
	m_RecvPool.clear();
	m_SendPool.clear();
	return true;
}
bool	TNetwork::Release() 
{
	ShutDown();	
	return true;
}

TNetwork::TNetwork()
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
	m_pModel = nullptr;
}


TNetwork::~TNetwork()
{
	WSACleanup();
}
