#include "TNetUser.h"
#include "TServer.h"

// |p1------p1e:p2-------p2e:p3-------t----p3e-------|
// 1, 패킷 헤더 > dwTrans
// 2, 패킷 헤더 == dwTrans
// 3, 패킷 헤더 < dwTrans
	// 3.1, 패킷 크기 == dwTrans
	// 3.2, 패킷 크기 > dwTrans
	// 3.3, 패킷 크기 < dwTrans

//0    ~    10000
//  50byte
// --------(s)9985,9990   10;
// 임시버퍼 받는 잔여량을 저장
//clear   0    ~    10000
// 주 버퍼에 임시버퍼의 잔여량 데이터 복사
//clear  |0~5: 50byte   ~    10000
bool TNetUser::DispatchRead(DWORD dwTrans, OVERLAPPED2* ov)
{
	delete ov;

	if (m_iWritePos + dwTrans >= g_iMaxDataBufferSize)
	{
		if (m_iReadPos > 0)
		{
			memmove(m_szDataBuffer, &m_szDataBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}
	// 받은 데이터 누적
	memcpy(&m_szDataBuffer[m_iWritePos], m_szRecvBuffer, dwTrans);
	m_iWritePos += dwTrans;
	m_iReadPos += dwTrans;

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
		// 1개의 패킷 이상의 데이터가 존재한다.
		if (packet->ph.len <= m_iReadPos)
		{
			do {
				TPacket tPacket;
				tPacket.pUser = this;
				memcpy(&tPacket.packet,
					&m_szDataBuffer[m_iPacketPos],
					packet->ph.len);
				// 페킷 풀에 완성 패킷을 넣어주어야 한다.
				m_pServer->m_RecvPacketPool.AddPacket(tPacket);

				m_iReadPos -= packet->ph.len;
				m_iPacketPos += packet->ph.len;

				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				UPACKET* packet = (UPACKET*)&m_szDataBuffer[m_iPacketPos];
			} while (packet->ph.len <= m_iReadPos);
		}
	}
	WaitReceive();
	return true;
}
bool TNetUser::DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov)
{
	delete ov;
	return true;
}
OVERLAPPED2* TNetUser::OverlappedRecv(int type)
{
	OVERLAPPED2* ov = new OVERLAPPED2(type);
	ZeroMemory(m_szRecvBuffer, g_iRecvBufferSize);
	m_wsaRecvBuffer.len = g_iRecvBufferSize;
	m_wsaRecvBuffer.buf = m_szRecvBuffer;
	return ov;
}
OVERLAPPED2* TNetUser::OverlappedSend(int type, UPACKET& msg)
{
	OVERLAPPED2* ov = new OVERLAPPED2(type);
	m_wsaSendBuffer.buf = (char*)&msg;
	m_wsaSendBuffer.len = msg.ph.len;
	return ov;
}
bool TNetUser::WaitReceive()
{
	OVERLAPPED2* ov = OverlappedRecv(OVERLAPPED2::MODE_RECV);
	DWORD cbTrans;
	DWORD dwflags = 0;
	int iRet = WSARecv(m_Sock, &m_wsaRecvBuffer, 1, 
		&cbTrans, &dwflags, (OVERLAPPED*)ov, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
bool TNetUser::WaitSender(UPACKET& msg)
{
	DWORD dwSendByte;
	int iRet;
	OVERLAPPED2* ov = OverlappedSend(OVERLAPPED2::MODE_SEND, msg);
	iRet = WSASend(m_Sock, &m_wsaSendBuffer, 1,	&dwSendByte, 0, (LPOVERLAPPED)ov, NULL);

	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
TNetUser::TNetUser(TServer* pServer) : m_pServer(pServer)
{
	m_bZoneSignin = false;
	m_bExit = false;
	m_iPacketPos = 0; // 페킷의 시작 주소
	m_iWritePos = 0; // 페킷 데이터 저장 주소
	m_iReadPos = 0; // 페킷 데이터 저장 주소
	m_dwCurrentServer = TServer::TSERVER_LOGIN;
}
TNetUser::~TNetUser()
{

}