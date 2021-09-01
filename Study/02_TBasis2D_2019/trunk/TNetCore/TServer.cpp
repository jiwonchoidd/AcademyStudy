#include "TServer.h"
#include "TSessionMgr.h"

HANDLE      TServer::g_hKillEvent;

bool TServer::PreRun()
{
	return true;
}
bool TServer::PostRun()
{
#pragma region m_SendPacketPool
	{
		m_SendPacketPool.Lock();
		std::vector<TPacket>::iterator senditer;
		for (senditer = m_SendPacketPool.m_list.begin();
			senditer != m_SendPacketPool.m_list.end();
			senditer++)
		{
			TNetUser* pUser = senditer->pUser;
			if (pUser->WaitSender(senditer->packet) == false)
			{
				SendLogoutUser(pUser);
			}
		}
		m_SendPacketPool.m_list.clear();
		m_SendPacketPool.Unlock();
	}
#pragma endregion m_SendPacketPool
#pragma region m_SendBroadcastPacketPool
	Broadcastting();
#pragma endregion m_SendBroadcastPacketPool
	return true;
}
void TServer::SetBind(SOCKET sock, ULONG_PTR key)
{
	m_pIocp->SetBind(sock, key);	
}
void TServer::SendLogoutUser(TNetUser* user) 
{
	UPACKET sendPacket;
	TNetResult ret;
	ret.id = user->m_Sock;
	ret.iRet = 1;
	MakePacket(sendPacket, (char*)&ret, sizeof(TNetResult), PACKET_LOGOUT_PLAYER);
	if (user != nullptr)
	{
		TPacket tPacket = { 0, };
		tPacket.pUser = user;
		tPacket.packet = sendPacket;
		m_RecvPacketPool.AddPacket(tPacket);
	}
	user->m_bExit = true;	
}
bool TServer::MakePacket(UPACKET& packet,
	char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
bool TServer::SendData(TNetUser& user, UPACKET& msg)
{
	DWORD dwSendByte;
	int iRet;
	user.m_wsaSendBuffer.buf = (char*)&msg;
	user.m_wsaSendBuffer.len = msg.ph.len;
	ZeroMemory(&user.m_ovSend, sizeof(OVERLAPPED));
	user.m_ovSend.iType = OVERLAPPED2::MODE_SEND;

	iRet = WSASend(user.m_Sock, &user.m_wsaSendBuffer, 1,
		&dwSendByte, 0, (LPOVERLAPPED)&user.m_ovSend, NULL);

	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
bool TServer::Run()
{
	while (m_bStarted)
	{
		if (WaitForSingleObject(TServer::g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		PreRun();
#pragma region m_RecvPacketPool
		m_RecvPacketPool.Lock();
			std::vector<TPacket>::iterator senditer;
			for (senditer = m_RecvPacketPool.m_list.begin();
				senditer != m_RecvPacketPool.m_list.end();
				senditer++)
			{
				UPACKET* packet = (UPACKET*)&senditer->packet;
				/*FunctionIterator calliter = m_fnExecutePacket.find(packet->ph.type);
				if (calliter != m_fnExecutePacket.end())
				{
					CallFuction call = calliter->second;
					(this->*call)(*senditer);
				}	*/
				FunctionIterator calliter = m_fnExecutePacket.find(packet->ph.type);
				if (calliter != m_fnExecutePacket.end())
				{
					CallFuction call = calliter->second;
					call(*senditer);
				}
			}
			m_RecvPacketPool.m_list.clear();
		m_RecvPacketPool.Unlock();
		// AddPacket 스위칭 될수 있기 때문에 Clear는 안에서 해야한다.
		//m_RecvPacketPool.Clear();

#pragma endregion m_RecvPacketPool

		PostRun();
	}
	return false;
}
bool TServer::Init()
{
	std::cout.sync_with_stdio(true);
	srand(time(NULL));
	g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	
	//m_fnExecutePacket[PACKET_CHAT_MSG] = &TServer::ChatMsg;
	//m_fnExecutePacket[PACKET_USER_POSITION] = &TServer::MoveAction;
	//m_fnExecutePacket[PACKET_LOGIN_REQ] = &TServer::Login;
	//m_fnExecutePacket[PACKET_ZONE_ENTRY_REQ] = &TServer::ZoneEntry;
	//m_fnExecutePacket[PACKET_LOGOUT_PLAYER] = &TServer::LogoutUser;
	//m_fnExecutePacket[PACKET_ATTACK_CHARACTER] = &TServer::AttackAction;

	///std::placeholders 설명
	///std::bind(함수 주소, 함수인자1, 함수인자2, 함수인자3, ...)
	///std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ... 등을 새로운 함수 인자와 맵핑 시킬수있다.
	///void Func(int iA, float fB, bool bC);
	///auto funcA = std::bind(Func, 10, 2.0f, true);funcA();
	///auto funcB = std::bind(Func, 10, std::placeholders::_1, true);funcB(2.0f);
	///auto funcC = std::bind(Func, std::placeholders::_2, std::placeholders::_1, true);funcC(2.0f, 10);
	///auto funcD = std::bind(Func, std::placeholders::_2, std::placeholders::_1, std::placeholders::_3);funcD(2.0f, 10, true);
	///그리고 전역함수가 아닌 클래스 함수의 경우
	///&ClassA::FuncA 행태를 바인드의 첫번쩨 인자로 사용한다
	
	/// 클래스 멤버 설명
	/// TServer server;
	/// std::function<void(TServer&, TPacket&)> fun = &TServer::ChatMsg;
	/// TPacket packet;
	/// fun(server, packet);

	m_fnExecutePacket[PACKET_CHAT_MSG] = bind(&TServer::ChatMsg, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_USER_POSITION] = bind(&TServer::MoveAction, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_LOGIN_REQ] = bind(&TServer::Login, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_ZONE_ENTRY_REQ] = bind(&TServer::ZoneEntryReq, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_ZONE_NEW_PLAYER] = bind(&TServer::ZoneEntry, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_LOGOUT_PLAYER] = bind(&TServer::LogoutUser, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_ATTACK_CHARACTER] = bind(&TServer::AttackAction, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_CREATE_ACCOUNT_REQ] = bind(&TServer::CreateAccount, this, std::placeholders::_1);
	
	// TODO : TQuery 초기화
	if (!m_Query.Init())
	{
		return false;
	}
	if (!m_Query.Connect(0))
	{
		return false;
	}
	m_Query.ExecPrepare(m_Query.m_hUserListStmt, true);
	return false;
}
bool TServer::Broadcastting()
{
	{
		TLock lock((TServerObj*)m_pSessionMgr);
		m_SendBroadcastPacketPool.Lock();

		std::map<SOCKET, TNetUser*>::iterator iter;
		for (iter = m_pSessionMgr->m_ZoneUserList.begin();
			iter != m_pSessionMgr->m_ZoneUserList.end();
			iter++)
		{
			TNetUser* pUser = iter->second;
			//if (pUser->m_dwCurrentServer == TServer::TSERVER_ZONE)
			{
				std::vector<TPacket>::iterator senditer;
				for (senditer = m_SendBroadcastPacketPool.m_list.begin();
					senditer != m_SendBroadcastPacketPool.m_list.end();
					senditer++)
				{
					if (pUser->WaitSender(senditer->packet) == false)
					{
						SendLogoutUser(pUser);
					}
				}
			}
		}
		m_SendBroadcastPacketPool.m_list.clear();
		m_SendBroadcastPacketPool.Unlock();
	}
	return true;
}
bool TServer::Release()
{
	if (!m_Query.Release())
	{
		return false;
	}
	::SetEvent(g_hKillEvent);
	return true;
}
TServer::TServer()
{
		
}

TServer::~TServer()
{
	TObjectPool<OVERLAPPED2>::AllFree();
	CloseHandle(g_hKillEvent);
}
