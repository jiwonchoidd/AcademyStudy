#include "TStreamPacket.h"
#include "TServer.h"


void TStreamPacket::Put(char* recvBuffer, 
						int iRecvSize, 
						TUser* pUser )
{
	memcpy(&m_strBuffer[m_iWritePos],
		    recvBuffer, 
			iRecvSize );
	m_iWritePos += iRecvSize;
	// 주소가 일치된다.
	m_pPacketData = (P_UPACKET)m_strBuffer;
	// 완성된 패킷이 도착했는냐?
	if( m_iWritePos >= m_pPacketData->ph.len )
	{
		// 경우1: 받은 데이터가 1개의 패킷 보다 많을 경우
		// 경우2: 여러개의 패킷을 받았을 경우
		// 두 경우 모두 완성 패킷을 만들고 
		//         나머지를 저장해야 한다.
		// 경우3: 정확하게 받은 데이터가 1개의 패킷일 경우
		do{
			T_PACKET addPacket;	
			ZeroMemory( &addPacket, sizeof(addPacket));
			memcpy(&addPacket.packet, m_strBuffer, m_pPacketData->ph.len );
			addPacket.pUser = pUser;

			pUser->AddPacket(addPacket);
		
			m_iWritePos -= m_pPacketData->ph.len;
			// 경우 3
			if( addPacket.packet.ph.len == iRecvSize ) break;

			m_iRecvPos += addPacket.packet.ph.len;
			// 다음 패킷의 시작 주소가 된다.
			m_pPacketData = (P_UPACKET)&m_strBuffer[m_iRecvPos];
		}while(m_iWritePos > m_pPacketData->ph.len);

		// 나머지 데이터 처리
		char strTemp[4096] = {0,};
		memcpy( strTemp, &m_strBuffer[m_iRecvPos], m_iWritePos );
		ZeroMemory(&m_strBuffer, sizeof(char)*4096);
		memcpy(&m_strBuffer, strTemp, m_iWritePos );
		m_pPacketData = (P_UPACKET)m_strBuffer;
		m_iRecvPos = 0;
	}	
};
TStreamPacket::TStreamPacket(void)
{
	m_iRecvPos = m_iWritePos = 0;
}


TStreamPacket::~TStreamPacket(void)
{
}
