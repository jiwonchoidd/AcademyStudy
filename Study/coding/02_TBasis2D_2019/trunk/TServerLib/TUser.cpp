#include "TUser.h"
#include "TServer.h"

bool TUser::Dispatch(DWORD dwTransfer, OVERLAPPED2* ov)
{
	{
		TLock lock(this);
		delete ov;
		if (ov->iFlag == PACK_SEND)
		{
			//std::cout << "SEND[" << dwTransfer << "]" << std::endl;
		}
		if (ov->iFlag == PACK_RECV)
		{
			Put(m_szRecvBuffer, dwTransfer);
			return WaitForReceive();
		}
	}
	return true;
}
void TUser::Put(char* pBuffer, DWORD dwByte)
{
	//m_dwWritePos 다음 버퍼 위치
	//m_dwStartPos 패킷의 시작 위치
	//m_dwReadPos  받은 데이터 크기
	if (m_dwWritePos + dwByte >= MAX_STREAM_SIZE)
	{
		if (m_dwReadPos > 0)
		{
			memmove(m_szPackStream, &m_szPackStream[m_dwStartPos], m_dwReadPos);
		}
		m_dwStartPos = 0;
		m_dwWritePos = m_dwReadPos;
	}
	memcpy(&m_szPackStream[m_dwWritePos], pBuffer, dwByte);
	m_dwWritePos += dwByte;
	m_dwReadPos += dwByte;

	if (m_dwReadPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* pPacket =	(UPACKET*)&m_szPackStream[m_dwStartPos];
		// 받은 데이터 크기(1개의 패킷보다 크면)
		if (pPacket->ph.len <= m_dwReadPos)
		{
			do {
				T_PACKET tPacket;
				ZeroMemory(&tPacket, sizeof(tPacket));
				tPacket.pUser = this;
				memcpy(&tPacket.packet,&m_szPackStream[m_dwStartPos],pPacket->ph.len);
				I_Server.m_RecvPool.Push(tPacket);

				m_dwReadPos -= pPacket->ph.len;
				m_dwStartPos += pPacket->ph.len;
				if (m_dwReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szPackStream[m_dwStartPos];
				if (pPacket->ph.type == 0)
				{
					break;
				}
			} while (pPacket->ph.len <= m_dwReadPos);
		}
	}
}
bool TUser::WaitForReceive()
{
	DWORD dwTransfer, flag = 0;

	recvBuffer.buf = m_szRecvBuffer;
	recvBuffer.len = MAX_USER_BUFFER_SIZE;

	OVERLAPPED2* pOV = new OVERLAPPED2();
	pOV->iFlag = PACK_RECV;

	int iRet = WSARecv(sock,
		&recvBuffer, 1,
		&dwTransfer, &flag,
		(LPOVERLAPPED)pOV, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			int iError = WSAGetLastError();
			//E_MSG("Server::WSARecv");			
			return false;
		}
	}
	return true;
}
TUser::TUser()
{
	bConnect = false;
	m_iRecvByte = 0;
	m_dwStartPos = 0;
	m_dwReadPos = 0;
	m_dwWritePos = 0;
	m_dwPacketCnt = 0;
}
TUser::~TUser()
{

}