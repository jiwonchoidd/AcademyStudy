#include "TServerStd.h"
#include "Protocol.h"
#include <conio.h>
char username[] = "홍길동";
bool g_bChatting = false;
bool g_bExit = false;

bool RecvMsg(SOCKET sock, char* pBuffer, int iNumDataByte)
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
bool   SendMsg(SOCKET sock, int type, char* data, int iSize)
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
DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	while (g_bExit==false)
	{
		char recvbuf[256] = { 0, };
		UPACKET packet;
		ZeroMemory(&packet, sizeof(packet));

		int iRecvByte = recv(sock, (char*)&packet.ph, PACKET_HEADER_SIZE, 0);
		if (iRecvByte == 0)
		{
			break;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			printf("\n서버 종료됨");
			break;
		}
		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			int iNumDataByte = packet.ph.len - PACKET_HEADER_SIZE;
			if (false == RecvMsg(sock, packet.msg, iNumDataByte))
			{
				break;
			}		
			iRecvByte = 0;

			switch (packet.ph.type)
			{
				case PACKET_DRUP_USERS_SC:
				{
					USER_BASE userinfo;
					ZeroMemory(&userinfo, sizeof(USER_BASE));
					memcpy(&userinfo, packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
					printf("%s:님이 퇴장하였습니다.", userinfo.szName);
				}break;
				case PACKET_DRUP_SC_ACK:
				{
					g_bExit = true;
					g_bChatting = false;
					printf("\n채팅방 퇴장.");
				}break;
				case PACKET_JOIN_SIGNIN_SC:
				{
					g_bChatting = true;
					printf("\n채팅방 입장.\n");
				}break;
				case  PACKET_CHAR_MSG:
				{
					USER_CHAT_MSG message;
					ZeroMemory(&message, sizeof(USER_CHAT_MSG));
					memcpy(&message, packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
					printf("\n[%s]%s", message.szName, message.msg);
				}break;
				case  PACKET_CHAR_NAME_SC_REQ:
				{				
					USER_BASE data;
					ZeroMemory(&data, sizeof(USER_BASE));
					data.iIndex = 8;
					memcpy(data.szName, username, strlen(username));
					SendMsg(sock, PACKET_CHAR_NAME_CS_ACK, (char*)&data,sizeof(USER_BASE) );
				}break;
			}
		}
		
	}
	closesocket(sock);
	return 1;
}
DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	int iEnd = 0;
	while (g_bExit==false)
	{
		if (g_bChatting == false)
		{
			continue;
		}
		printf("%s:", username);
		fgets(buf, 256, stdin);
		if (buf[0] == '\n')
		{
			if (false == SendMsg(sock, PACKET_DRUP_CS_REQ, nullptr, 0))
			{
				E_MSG("send");
				break;
			}	
			continue;
		}
		int iMsgSize = strlen(buf);
		
		USER_CHAT_MSG message;
		ZeroMemory(&message, sizeof(USER_CHAT_MSG));
		memcpy(&message.szName, username, strlen(username));
		memcpy(&message.msg, buf, strlen(buf));

		if (false ==SendMsg(sock, PACKET_CHAR_MSG, (char*)&message, USER_NAME_SIZE + iMsgSize))
		{
			E_MSG("send");
			break;
		}			
	}
	closesocket(sock);
	return 0;
}

void main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	unsigned long iMode = 0;
	ioctlsocket(sock, FIONBIO, &iMode);

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = inet_addr("192.168.0.8");
	int ret = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		E_MSG("connect");
		return;
	}
	DWORD iThreadID;
	HANDLE hThreadSend = CreateThread(0, 0, 
		SendThread, 
		(void*)sock, 
		0, 
		&iThreadID);

	DWORD iThreadIDA;
	HANDLE hThreadRecv = CreateThread(0, 0,
		RecvThread,
		(void*)sock,
		0,
		&iThreadIDA);	
	
	// 대기함수	
	WaitForSingleObject(hThreadSend, INFINITE);
	WaitForSingleObject(hThreadRecv, INFINITE);

	CloseHandle(hThreadSend);
	CloseHandle(hThreadRecv);
	WSACleanup();
}