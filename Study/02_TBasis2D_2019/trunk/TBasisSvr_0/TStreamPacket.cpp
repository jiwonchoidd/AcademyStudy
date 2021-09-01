#include "TStreamPacket.h"
#include "TDebugString.h"
#include "TServer.h"

void TStreamPacket::Put(char* recvBuffer, int iRecvSize, TUser* pUser )
{
	memcpy(&m_strBuffer[m_iWritePos], recvBuffer, sizeof(char)*iRecvSize );
	m_iWritePos += iRecvSize;
	// 주소가 동일
	m_pPacketData = (P_UPACKET)m_strBuffer;

	// 패킷 크기보다는 받은 데이터가 크거나 같다.
	if( m_iWritePos >= m_pPacketData->ph.len )
	{
		static int iCount = 0;
	
		do{
			iCount++;
			// 경우1:받은 데이터가 1개의 패킷보다 많을 경우.
			// 경우2:여러개의 패킷의 데이터를 받았을 경우.
			// 경우1 & 경우2 = 받드시 나머지 패킷 데이터를 저장해야 한다.
			memcpy( &m_PacketData, m_strBuffer, m_pPacketData->ph.len );

			T_PACKET  addPacket;
			addPacket.packet = m_PacketData;
			addPacket.pUser  = pUser;
			I_Server.m_StreamPacket.m_PacketList.push_back( addPacket );

			ZeroMemory( &m_PacketData, sizeof(UPACKET));

			m_iWritePos -= m_pPacketData->ph.len;
			// 정확한 크기가 들어와서 1개의 패킷을 완성했을 경우 거의 이런 경우는 없다.
			if( addPacket.packet.ph.len == iRecvSize ) break;

			m_iRecvPos += addPacket.packet.ph.len;
			m_pPacketData = (P_UPACKET)&m_strBuffer[m_iRecvPos];//addPacket.ph.len];

		}while(m_iWritePos >= m_pPacketData->ph.len);

		I_DebugStr.DisplayText("%d\r\n", iCount );
		iCount = 0;
		TCHAR strTmp[256]={0,};
		memcpy(strTmp, &m_strBuffer[m_iRecvPos],m_iWritePos);
		ZeroMemory( &m_strBuffer, sizeof(char)* 2048);
		memcpy( &m_strBuffer, &strTmp, m_iWritePos );
		m_pPacketData = (P_UPACKET)m_strBuffer;
		m_iRecvPos = 0;
	}
	else
	{
		//경우3:완성된 패킷을 만들기 부족한 데이터가 있을 경우.
		I_DebugStr.DisplayText("%d\r\n", m_iWritePos );
	}
}

TStreamPacket::TStreamPacket(void)
{
	m_iWritePos = 0;
	m_iRecvPos	= 0;
}

TStreamPacket::~TStreamPacket(void)
{
}
