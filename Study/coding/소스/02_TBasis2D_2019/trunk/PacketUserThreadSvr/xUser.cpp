#include "xUser.h"

void CALLBACK CompletionRoutine(
	DWORD dwError,
	DWORD cbTrans,
	LPWSAOVERLAPPED lpOV,
	DWORD dwFlags)
{
	TOV* tOV = (TOV*)lpOV;
	xUser* pUser = (xUser*)tOV;
	if (tOV->iType == 0)
	{
		int iRecvByte = pUser->AddBuffer(cbTrans);
		pUser->Recv(iRecvByte);
	}
}
DWORD xUser::Recv(DWORD dwSize)
{
	ZeroMemory(&m_ovRecv, sizeof(TOV));
	DWORD dwRecvBytes;
	DWORD dwFlags = 0;
	m_wsaWritePos.buf = m_pWritePos;
	m_wsaWritePos.len = dwSize;
	int dwRet = WSARecv(sock,
		&m_wsaWritePos, 1,
		&dwRecvBytes,
		&dwFlags,
		&m_ovRecv,
		CompletionRoutine);
	if (dwRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return 0;
		}
	}
	/*else
	{

	}*/
	return dwRet;
}

xUser::xUser(const xUser& user)
{
	sock = user.sock;
	address = user.address;
	ZeroMemory(m_RecvBuffer, PACKET_MAX_SIZE);
	m_pWritePos = m_RecvBuffer;
	m_iStartPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
}
xUser::xUser()
{
	ZeroMemory(m_RecvBuffer, PACKET_MAX_SIZE);
	m_pWritePos = m_RecvBuffer;
	m_iStartPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;
}
int  xUser::AddBuffer(int iRecvData)
{
	int iRetRecvByte = 0;
	if (m_iWritePos < PACKET_HEADER_SIZE)
	{		
		m_iWritePos += iRecvData;
		m_pWritePos = &m_RecvBuffer[m_iWritePos];
		if (m_iWritePos == PACKET_HEADER_SIZE)
		{
			m_pRecvPacket = (UPACKET*)m_RecvBuffer;
			iRetRecvByte = m_pRecvPacket->ph.len - PACKET_HEADER_SIZE;
		}
		else
		{
			iRetRecvByte = PACKET_HEADER_SIZE - iRecvData;
		}		
	}
	else
	{
		m_iWritePos += iRecvData;
		m_pWritePos = &m_RecvBuffer[m_iWritePos];

		if (m_iWritePos == m_pRecvPacket->ph.len)
		{
			UPACKET packet;
			memcpy(&packet, m_pRecvPacket,
				sizeof(UPACKET));

			TNetwork::m_RecvPool.push_back(packet);
			std::cout << "TNetwork::m_RecvPool" << std::endl;

			m_iWritePos = 0;
			ZeroMemory(m_RecvBuffer, PACKET_MAX_SIZE);
			m_pWritePos = &m_RecvBuffer[m_iWritePos];
			iRetRecvByte = PACKET_HEADER_SIZE;
		}
		else
		{
			iRetRecvByte = m_pRecvPacket->ph.len - m_iWritePos;
		}
	}
	return iRetRecvByte;
}
