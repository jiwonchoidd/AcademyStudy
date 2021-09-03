#include "TSelectModel.h"
#include "TNetwork.h"
bool TSelectModel::RecvData()
{
	if (m_iRecvByte < PACKET_HEADER_SIZE)
	{
		int iRecvByte = recv(m_Sock, 
			&m_szRecvBuffer[m_iRecvByte], PACKET_HEADER_SIZE - m_iRecvByte, 0);
		if (iRecvByte == 0)
		{
			return false;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() !=
				WSAEWOULDBLOCK)
			{
				E_MSG("recv");
				return false;
			}
		}
		else
		{
			m_iRecvByte += iRecvByte;
			UPACKET* uPacket = (UPACKET*)m_szRecvBuffer;
			if (uPacket->ph.len == m_iRecvByte)
			{
				TNetwork::m_RecvPool.push_back(*uPacket);
				m_iRecvByte = 0;
			}
		}
	}
	else
	{
		UPACKET* uPacket = (UPACKET*)m_szRecvBuffer;
		int iRecvByte = recv(m_Sock,
			&m_szRecvBuffer[m_iRecvByte],
			uPacket->ph.len - m_iRecvByte, 0);
		if (iRecvByte == 0)
		{
			return false;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() !=
				WSAEWOULDBLOCK)
			{
				E_MSG("recv");
				return false;
			}
		}

		m_iRecvByte += iRecvByte;
		// 1개의 패킷 완성
		if (uPacket->ph.len == m_iRecvByte)
		{
			TNetwork::m_RecvPool.push_back(*uPacket);
			m_iRecvByte = 0;
		}
	}
	return true;
}
bool TSelectModel::RecvMsg(SOCKET sock, char* pBuffer, int iNumDataByte)
{
	if (iNumDataByte == 0) return true;
	int iRecvByte = 0;
	do {
		int iByte = recv(sock, &pBuffer[iRecvByte], iNumDataByte - iRecvByte, 0);
		if (iByte == 0)
		{
			return false;
		}
		if (iByte == SOCKET_ERROR)
		{
			return false;
		}
		iRecvByte += iByte;
	} while (iNumDataByte > iRecvByte);
	return true;
}
bool   TSelectModel::SendMsg(SOCKET sock, int type, char* data, int iSize)
{
	UPACKET packet;
	ZeroMemory(&packet, sizeof(UPACKET));
	packet.ph.type = type;
	packet.ph.len = PACKET_HEADER_SIZE + iSize;
	if (data != nullptr)
	{
		memcpy(packet.msg, data, iSize);
	}

	int iSendByte = 0;
	do {
		int iPacketByte = send(sock,
			(char*)&packet, packet.ph.len - iSendByte, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			return false;
		}
		iSendByte += iPacketByte;
	} while (packet.ph.len > iSendByte);
	return true;
}
bool  TSelectModel::SendMsg(UPACKET& packet)
{	
	int iSendByte = 0;
	do {
		int iPacketByte = send(m_Sock,
			(char*)&packet, packet.ph.len - iSendByte, 0);
		if (iPacketByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() !=
				WSAEWOULDBLOCK)
			{
				E_MSG("send");
				return false;
			}			
		}
		else
		{
			if (iPacketByte >= 0)
			{
				iSendByte += iPacketByte;
			}
		}
	} while (packet.ph.len > iSendByte);
	return true;
}
TSelectModel::TSelectModel()
{
	m_iRecvByte = 0;	
}


TSelectModel::~TSelectModel()
{
}
