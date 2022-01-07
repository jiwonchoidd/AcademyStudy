#include "KNetwork.h"

//서버 Select 모델
std::list<KNetworkUser> userlist;
int SendMsg(SOCKET sock, char* msg, WORD type)
{
	//1. 패킷을 객체 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	//패킷의 길이는 헤더 사이즈 + 메세지 사이즈
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	//메세지 받은걸로 memcpy로 패킷 만듬
	memcpy(packet.msg, msg, strlen(msg));

	// 2번 패킷 전송 : 운영체제 sendbuffer(short바이트)
	char* pMsg = (char*)&packet;
	int iSize = 0;

	do {
		//TCP는 연속적이지만 바이트 단위로 나눠져 보내질도 있음
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
	return iSize;
}
//패킷을 받았을때.
int SendMsg(SOCKET sock, UPACKET& packet)
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
	return iSize;
}
int AddUser(SOCKET sock)
{
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	SOCKET clientSock = accept(sock,
		(sockaddr*)&clientAddr, &iLen);
	if (clientSock == SOCKET_ERROR)
	{
		return -1;
	}
	else
	{
		KNetworkUser user;
		user.set(clientSock, clientAddr);
		userlist.push_back(user);
		std::cout
			<< "ip =" << inet_ntoa(clientAddr.sin_addr)
			<< "port =" << ntohs(clientAddr.sin_port)
			<< "  " << std::endl;
		std::cout << userlist.size() << " 명 접속중.." << std::endl;
	}
	return 1;
}
int RecvUser(KNetworkUser& user)
{
	char szRecvBuffer[1024] = { 0, };
	int iRecvByte = recv(user.m_Sock, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		return -1;
	}
	user.DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}
void main()
{
	KNetwork net;
	net.InitNetwork();
	net.InitServer(SOCK_STREAM, 10000);

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout<< "Server Start." << std::endl;

	FD_SET	rSet;
	FD_SET	wSet;
	timeval timeOut;

	timeOut.tv_sec = 1;
	timeOut.tv_usec = 0;

	while (1)
	{
		//매번 초기화 구조체의 주요 변수는 배열 
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		//리슨 소켓에 rset를 저장함
		FD_SET(net.m_ListenSocket, &rSet);

		std::list<KNetworkUser>::iterator useriter;
		for (useriter = userlist.begin();
			useriter != userlist.end();)
		{
			//유저 중에 연결 끊긴 사람이 있다면 리스트에서 삭제함
			if ((*useriter).m_bConnect == false)
			{
				std::cout << (*useriter).m_csName << " 접속종료됨." << std::endl;
				useriter = userlist.erase(useriter);
				continue;
			}
			//유저의 소켓을 rset에 저장함
			FD_SET((*useriter).m_Sock, &rSet);
			// 만약 user에서 보낸 패킷이 있으면 wSet 구조체에 소켓 저장
			if ((*useriter).m_lPacketPool.size() > 0)
			{
				FD_SET((*useriter).m_Sock, &wSet);
			}
			useriter++;
		}
		//select 모델을 사용
		//select 함수는 fd_set 구조체에 할당한
		// FD의 이벤트가 발생하면 이를 감지하고
		// 어떤 FD 이벤트가 발생했는지 알려준다.
		int iRet = select(0,
			&rSet,
			&wSet,
			nullptr,
			&timeOut);

		//아무 활동이 없으면 0을 반환한다.
		std::cout << ".";
		if (iRet == 0)
		{
			continue;
		}
		//fdset중 소켓 fd에 해당하는 비트가 세트되어 있으면 양수값인 fd를 리턴한다.
		if (FD_ISSET(net.m_ListenSocket, &rSet))
		{
			if (AddUser(net.m_ListenSocket) <= 0)
			{
				break;
			}
		}
		//유저 리스트를 돌면서 해당 유저 소켓이 저장되어 있다면
		for (KNetworkUser& user : userlist)
		{
			//연결 끊겼는지 확인 여기서는 불값 체크만
			if (FD_ISSET(user.m_Sock, &rSet))
			{
				int iRet = RecvUser(user);
				if (iRet <= 0)
				{
					user.m_bConnect = false;
				}
			}
		}
		for (KNetworkUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &wSet))
			{
				if (user.m_lPacketPool.size() > 0)
				{
					std::list<KPacket>::iterator iter;
					for (iter = user.m_lPacketPool.begin();
						iter != user.m_lPacketPool.end(); )
					{
						for (KNetworkUser& senduser : userlist)
						{
							int iRet = SendMsg(senduser.m_Sock, (*iter).m_uPacket);
							if (iRet <= 0)
							{
								senduser.m_bConnect = false;
							}
						}
						iter = user.m_lPacketPool.erase(iter);
					}
				}
			}
		}
	}
	net.CloseNetwork();
}