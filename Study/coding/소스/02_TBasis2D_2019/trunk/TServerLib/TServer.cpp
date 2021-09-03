#include "TServer.h"
#include "TUserManager.h"
#include "TIocp.h"
bool TServer::SendMsg(TUser* pUser, int type, char* data, int iSize)
{
	UPACKET packet;
	ZeroMemory(&packet, sizeof(UPACKET));
	packet.ph.type = type;
	packet.ph.len = PACKET_HEADER_SIZE + iSize;
	if (data != nullptr)
	{
		memcpy(packet.msg, data, iSize);
	}
	return SendMsg(pUser, packet);
}
bool TServer::SendMsg(TUser* pUser, UPACKET& packet)
{
	if (pUser->bConnect == false)
	{
		closesocket(pUser->sock);
		return false;
	}
	int iSendByte = 0;
	pUser->sendBuffer.buf = (char*)&packet;
	pUser->sendBuffer.len = packet.ph.len;
	OVERLAPPED2* pOV = new OVERLAPPED2();
	pOV->iFlag = PACK_SEND;

	DWORD dwSendByte;
	int iRet = WSASend(pUser->sock,&pUser->sendBuffer, 1,&dwSendByte, 0,(LPOVERLAPPED)pOV,NULL);
	
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{			
			delete pOV;
			pUser->bConnect = false;
			closesocket(pUser->sock);
			//E_MSG("WaitForReceive");
			return false;
		}
	}
	return true;
}
void TServer::Broadcast(T_PACKET& tPacket)
{
	{
		TLock lock(this);
		for (TUser* user : I_UserMgr.m_UserList)
		{
			if (user->isChatting != true)continue;
			if (user->bConnect != true) continue;
			tPacket.pUser = user;
			m_SendPool.Push(tPacket);
		}
	}	
}
bool TServer::SendPool(TUser* pUser, int type, char* data, int iSize)
{
	{
		TLock lock(this);
		T_PACKET tPacket;
		tPacket.pUser = pUser;

		ZeroMemory(&tPacket.packet, sizeof(UPACKET));
		tPacket.packet.ph.type = type;
		tPacket.packet.ph.len = PACKET_HEADER_SIZE + iSize;
		if (data != nullptr)
		{
			memcpy(tPacket.packet.msg, data, iSize);
		}
		m_SendPool.Push(tPacket);
	}
	return true;
}
bool TServer::SendPool(TUser* pUser, UPACKET& packet)
{
	{
		TLock lock(this);
		T_PACKET tPacket;
		tPacket.pUser = pUser;
		memcpy(&tPacket.packet, &packet, packet.ph.len);
		m_SendPool.Push(tPacket);
	}
	return true;
}
bool TServer::SendPool(T_PACKET& packet)
{
	{
		TLock lock(this);
		m_SendPool.Push(packet);
	}
	return true;
}
bool TServer::Run()
{
	while (m_bStarted)
	{
		m_RecvPool.Lock();
		std::list<T_PACKET>& redvlist = m_RecvPool.Get();
		for (T_PACKET& tPacket : m_RecvPool.Get())
		{
			switch (tPacket.packet.ph.type)
			{
			case  PACKET_DRUP_CS_REQ:
			{
				tPacket.pUser->isChatting = false;
				USER_BASE info;
				info.iIndex = 9;
				memcpy(info.szName, tPacket.pUser->name.c_str(), tPacket.pUser->name.size());
				for (TUser* user : I_UserMgr.m_UserList)
				{
					if (user->isChatting != true) continue;
					if (user->bConnect != true) continue;
					SendPool(user, PACKET_DRUP_USERS_SC, (char*)&info, sizeof(USER_BASE));
				}
				SendPool(tPacket.pUser, PACKET_DRUP_SC_ACK, nullptr, 0);
			}break;
			case  PACKET_CHAR_MSG:
			{
				USER_CHAT_MSG msg;
				ZeroMemory(&msg, sizeof(USER_CHAT_MSG));
				memcpy(msg.szName, tPacket.pUser->name.c_str(), tPacket.pUser->name.size());
				memcpy(msg.msg, tPacket.packet.msg, tPacket.packet.ph.len - PACKET_HEADER_SIZE);				
				memcpy(tPacket.packet.msg, &msg, USER_NAME_SIZE + strlen(msg.msg) );
				tPacket.packet.ph.len = PACKET_HEADER_SIZE + USER_NAME_SIZE + strlen(msg.msg);
				Broadcast(tPacket);

				//std::cout << "[" << msg.szName << InterlockedIncrement64(&tPacket.pUser->m_dwPacketCnt) << "]" << msg.msg;
			}break;
			case  PACKET_CHAR_NAME_CS_ACK:
			{
				TUser* pUser = tPacket.pUser;
				USER_NAME_ACK userName;
				memcpy(&userName, tPacket.packet.msg, sizeof(USER_NAME_ACK));
				pUser->name = userName.szName;
				//std::cout << userName.szName << std::endl;
				// 클라이언트 등록 허가
				SendPool(pUser, PACKET_JOIN_SIGNIN_SC, nullptr, 0);
				// 다른 클라이언트 유저 접속 통지
				//PACKET_JOIN_USER_SC+data
				USER_BASE info;
				ZeroMemory(&info, sizeof(USER_BASE));
				info.iIndex = 9;
				memcpy(info.szName, userName.szName, USER_NAME_SIZE);
				for (TUser* user : I_UserMgr.m_UserList)
				{
					if (user->isChatting != true)continue;
					if (user->bConnect != true) continue;
					SendPool(user, PACKET_JOIN_USER_SC, (char*)&info, sizeof(USER_BASE));
				}
				pUser->isChatting = true;
			}break;
			}
		}
		m_RecvPool.Get().clear();
		m_RecvPool.UnLock();

		
		m_SendPool.Lock();
		std::list<T_PACKET>& list = m_SendPool.Get();
		for (T_PACKET& tPacket : m_SendPool.Get())
		{
			SendMsg(tPacket.pUser, tPacket.packet);
		}
		m_SendPool.Get().clear();
		m_SendPool.UnLock();	
		
		Sleep(1);
	}
	return true;
}
bool TServer::Start()
{
	I_IOCP.Run();
	return true;
}
bool TServer::Init()
{
	CreateThread();
	I_IOCP.Init();
	return m_Accepter.Set();
}
TServer::TServer()
{
}


TServer::~TServer()
{
}
