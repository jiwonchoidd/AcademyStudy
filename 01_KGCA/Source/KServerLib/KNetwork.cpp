#include "KNetwork.h"

//소켓 프로그램 시작 2.2
bool KNetwork::InitNetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	return true;
}

bool KNetwork::InitServer(int protocol, int iport, const char* ip)
{
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iport);
	if (ip == nullptr)
	{
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		sa.sin_addr.s_addr = inet_addr(ip);
	}

	//소켓에 주소 할당
	int iRet = bind(m_ListenSocket, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return false;

	//클라이언트로 부터 연결 요청을 처리할수있는 상태를 만듬 
	iRet = listen(m_ListenSocket, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return false;
	return true;
}
int KNetwork::SendMsg(SOCKET sock, char* msg, WORD type)
{
	//패킷 생성
	UPACKET	packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE; //ph가 헤더에는 크기와 타입을 알려줌
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	//운영체제 sendbuffer에 패킷 전송
	char* pMsg = (char*)&packet;
	int iSize = 0;
	do {
		//send함수
		int iSendByte = 0;
			iSendByte= send(sock, &pMsg[iSize],
			packet.ph.len - iSendByte, 0);

		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSize += iSendByte;
	} while (iSize < packet.ph.len);
	return iSize;
}
int KNetwork::SendMsg(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int iSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSize],
			packet.ph.len - iSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSize += iSendByte;
	} while (iSize < packet.ph.len);
	//크기만큼 보내지게 반복문
	return iSize;
}

//연결된 소켓으로부터 데이터를 수신함. 
//성공하면 1
int KNetwork::RecvUser(KNetworkUser& user)
{
	char szRecvBuffer[1024] = { 0, };
	//recv 받기
	int iRecvByte = recv(user.m_ListenSocket, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return 2;
	}
	user.DispatchRead(szRecvBuffer, iRecvByte);

	// 서버 텍스트 받은거 출력
	KChatting recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	*(user.m_lPacketPool.begin()) >> recvdata.index >> recvdata.name
		>> recvdata.damage >> recvdata.message;
	std::cout << recvdata.name<<" : "<< recvdata.message << std::endl;

	return 1;
}
KNetwork::KNetwork()
{
}
KNetwork::~KNetwork()
{
}
bool KNetwork::CloseNetwork()
{
	closesocket(m_ListenSocket);
	WSACleanup();
	return true;
}
