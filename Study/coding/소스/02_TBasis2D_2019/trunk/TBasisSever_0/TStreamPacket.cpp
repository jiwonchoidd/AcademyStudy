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
	// �ּҰ� ��ġ�ȴ�.
	m_pPacketData = (P_UPACKET)m_strBuffer;
	// �ϼ��� ��Ŷ�� �����ߴ³�?
	if( m_iWritePos >= m_pPacketData->ph.len )
	{
		// ���1: ���� �����Ͱ� 1���� ��Ŷ ���� ���� ���
		// ���2: �������� ��Ŷ�� �޾��� ���
		// �� ��� ��� �ϼ� ��Ŷ�� ����� 
		//         �������� �����ؾ� �Ѵ�.
		// ���3: ��Ȯ�ϰ� ���� �����Ͱ� 1���� ��Ŷ�� ���
		do{
			T_PACKET addPacket;	
			ZeroMemory( &addPacket, sizeof(addPacket));
			memcpy(&addPacket.packet, m_strBuffer, m_pPacketData->ph.len );
			addPacket.pUser = pUser;

			pUser->AddPacket(addPacket);
		
			m_iWritePos -= m_pPacketData->ph.len;
			// ��� 3
			if( addPacket.packet.ph.len == iRecvSize ) break;

			m_iRecvPos += addPacket.packet.ph.len;
			// ���� ��Ŷ�� ���� �ּҰ� �ȴ�.
			m_pPacketData = (P_UPACKET)&m_strBuffer[m_iRecvPos];
		}while(m_iWritePos > m_pPacketData->ph.len);

		// ������ ������ ó��
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
