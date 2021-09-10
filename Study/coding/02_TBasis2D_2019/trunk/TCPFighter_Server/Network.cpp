
/*---------------------------------------------------------------

select ���� ���� ��Ʈ��ũ ���� �Լ�.

������� ������ ����Ѵ�.

---------------------------------------------------------------*/
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "AyaPacket.h"
#include "AyaStreamSQ.h"
#include "Network.h"
#include "PacketProc.h"

#include "PacketDefine.h"


// =================================================================
// ��Ʈ��ũ ���� ���� ����.
//
// =================================================================
//---------------------------------------------------------------
// ���� �� ��Ŷ ���� ����.
//---------------------------------------------------------------
SOCKET				g_netListenSocket;

//---------------------------------------------------------------
// ������ ���� �迭.
//---------------------------------------------------------------
st_NETWORK_SESSION	g_netSession[dfNETWORK_MAX_USER];

//---------------------------------------------------------------
// ������ ID �Ҵ�.
//---------------------------------------------------------------
DWORD				g_netSessionID = 1;



////////////////////////////////////////////////////////////////////
// ����ִ� ���� ã��.
//
// Parameters: ����.
// Return: (st_NETWORK_SESSION *)���� ����ü.
////////////////////////////////////////////////////////////////////
st_NETWORK_SESSION		*GetEmptySession(void)
{
	st_NETWORK_SESSION	*stpSession = NULL;
	int					iCount = 0;

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( !g_netSession[iCount].bConnect )
			return &g_netSession[iCount];
	}

	return NULL;
}


////////////////////////////////////////////////////////////////////
// ID �� ���� ã��.
//
// Parameters: (DWORD)SessionID.
// Return: (st_NETWORK_SESSION *)���� ����ü.
////////////////////////////////////////////////////////////////////
st_NETWORK_SESSION		*GetSession(DWORD dwSessionID)
{
	st_NETWORK_SESSION	*stpSession = NULL;
	int					iCount = 0;

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( dwSessionID == g_netSession[iCount].dwSessionID )
			return &g_netSession[iCount];
	}

	return NULL;
}


////////////////////////////////////////////////////////////////////
// Socket ���� ���� ã��.
//
// Parameters: (SOCKET)Socket.
// Return: (st_NETWORK_SESSION *)���� ����ü.
////////////////////////////////////////////////////////////////////
st_NETWORK_SESSION		*GetSession(SOCKET Socket)
{
	st_NETWORK_SESSION	*stpSession = NULL;
	int					iCount = 0;

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( Socket == g_netSession[iCount].Socket )
			return &g_netSession[iCount];
	}

	return NULL;
}




////////////////////////////////////////////////////////////////////
// WinSock �ʱ�ȭ.
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netStartUp(void)
{
	WORD		wVer;
	WSADATA		wsaData;
	int			iCount;

	wVer = MAKEWORD(2,2);

	//---------------------------------------------------------------
	// ���� ����.
	//---------------------------------------------------------------
	if ( 0 != WSAStartup(wVer, &wsaData) )
		return FALSE;


	//---------------------------------------------------------------
	// TCP ���� ����.
	//---------------------------------------------------------------
	g_netListenSocket = socket(AF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == g_netListenSocket )
	{
		return FALSE;
	}

	//---------------------------------------------------------------
	// ���Ͽɼ� ����.
	// ���� ����ӵ��� ���ؼ� Nagle �˰��� ����.
	// ���۸��� ���� �����Ƿ� ��ŷ��� �þ�� �ȴ�.
	//---------------------------------------------------------------
	char cFlag = 1;
	if ( SOCKET_ERROR == setsockopt(g_netListenSocket, IPPROTO_TCP, TCP_NODELAY, &cFlag, sizeof(char)) )
	{
		return FALSE;
	}


	//---------------------------------------------------------------
	// ���ǿ� ���� ���� �ʱ�ȭ.
	//---------------------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		g_netSession[iCount].bConnect = FALSE;
		g_netSession[iCount].Socket = INVALID_SOCKET;
		g_netSession[iCount].dwSessionID = 0;

		g_netSession[iCount].clRecvQ.ClearBuffer();
		g_netSession[iCount].clSendQ.ClearBuffer();

		g_netSession[iCount].dwTrafficTick = timeGetTime();
		g_netSession[iCount].dwTrafficCount = 0;
	}

	LogOutput("WSAStartup #");

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// WinSock ����
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netCleanUp(void)
{
	int iCount;

	closesocket(g_netListenSocket);
	g_netListenSocket = INVALID_SOCKET;

	//---------------------------------------------------------------
	// ��� ���ǿ� ���� Ŭ���� ���� ȣ��.
	//---------------------------------------------------------------
	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( g_netSession[iCount].bConnect )
			closesocket(g_netSession[iCount].Socket);
	}

	WSACleanup();

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// PortOpen
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netPortOpen(void)
{
	//-----------------------------------------------------
	// � IP, � ��Ʈ�� �������� ���ε��� ���� �ּҰ� ����.
	//-----------------------------------------------------
	SOCKADDR_IN Addr;
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(dfNETWORK_PORT);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//-----------------------------------------------------
	// ��Ʈ3000 ���� �����Ѵ�.
	//-----------------------------------------------------
	if ( SOCKET_ERROR == bind(g_netListenSocket, (SOCKADDR *)&Addr, sizeof(SOCKADDR_IN)) )
		return FALSE;

	LogOutput("Bind OK # Port:%d", dfNETWORK_PORT);

	//-----------------------------------------------------
	// ��Ʈ ���� !!
	//-----------------------------------------------------
	if ( SOCKET_ERROR == listen(g_netListenSocket, SOMAXCONN) )
		return FALSE;


	LogOutput("Listen OK #");

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// ��Ʈ��ũ ���� ó��.
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netNetworkProc(void)
{
	fd_set	RecvSet;
	fd_set	SendSet;

	timeval	Time;
	int		iCount;
	int		iResult;

	//-----------------------------------------------------
	// select �� Recv, Send üũ�� ���� ���� ����.
	//-----------------------------------------------------
	FD_ZERO(&RecvSet);
	FD_ZERO(&SendSet);
	FD_SET(g_netListenSocket, &RecvSet);

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( g_netSession[iCount].bConnect )
		{
			//-----------------------------------------------------
			// �������� ������ �ִٸ� Recv �˻�����.
			//-----------------------------------------------------
			FD_SET(g_netSession[iCount].Socket, &RecvSet);

			//-----------------------------------------------------
			// �ش� ���ǿ� SendQ �� �����Ͱ� �ִٸ�
			// SendSet ���� ����� ������.
			//
			// Send ���ɿ��θ� �޾Ƽ� Send �۾��� ����� �ϹǷ�.
			//-----------------------------------------------------
			if ( 0 < g_netSession[iCount].clSendQ.GetUseSize() )
				FD_SET(g_netSession[iCount].Socket, &SendSet);
		}
	}

	//-----------------------------------------------------
	// Select ���ð� ����. 
	//-----------------------------------------------------
	Time.tv_sec = 0;
	Time.tv_usec = 1;

	//-----------------------------------------------------
	// select �Լ��� ȣ���Ͽ� �˻�.
	//-----------------------------------------------------
	iResult = select(0, &RecvSet, &SendSet, 0, &Time);

	//-----------------------------------------------------
	// ���ϰ��� 0 ���� ũ�Ƿ�, ������ ������ �Դ�.  ã��.
	//-----------------------------------------------------
	if ( 0 < iResult )
	{
		//-----------------------------------------------------
		// ���ӿ�û ó��.
		//-----------------------------------------------------
		if ( FD_ISSET(g_netListenSocket, &RecvSet) )
		{
			netAcceptEvent();
		}

		//-----------------------------------------------------
		// ���� ������ ���Ͽ��� �� ������ �˻�����.
		//-----------------------------------------------------
		for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
		{
			if ( g_netSession[iCount].bConnect )
			{
				//-----------------------------------------------------
				// Recv ������ �ִ���...
				//-----------------------------------------------------
				if ( FD_ISSET(g_netSession[iCount].Socket, &RecvSet) )
				{
					if ( !netRecvEvent(iCount) )
					{
						netRemoveSession(g_netSession[iCount].dwSessionID);
					}
				}
				//-----------------------------------------------------
				// Send ������ �ִ���...
				//-----------------------------------------------------
				else if ( FD_ISSET(g_netSession[iCount].Socket, &SendSet) )
				{
					if ( !netSendEvent(iCount) )
					{
						netRemoveSession(g_netSession[iCount].dwSessionID);
					}
				}
			}
		}

	}
	else if ( SOCKET_ERROR  == iResult )
	{
		//-----------------------------------------------------
		// ����, ������. 
		//-----------------------------------------------------
		return FALSE;
	}

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// ����� ���� �̺�Ʈ ó��.
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netAcceptEvent(void)
{
	SOCKADDR_IN	Addr;
	SOCKET		Socket;
	int			iSize = sizeof(Addr);

	st_NETWORK_SESSION *stpSession;

	//-----------------------------------------------------
	// �ϴ� �������� ������ �޾Ƶ��δ�.
	//-----------------------------------------------------
	Socket = accept(g_netListenSocket, (SOCKADDR *)&Addr, &iSize);
	
	if ( INVALID_SOCKET == Socket )
		return FALSE;

	//-----------------------------------------------------
	// ����ִ� Ŭ���̾�Ʈ ��ġ�� ã��.
	//-----------------------------------------------------
	stpSession = GetEmptySession();

	if ( NULL == stpSession )
	{
		LogOutput("ConnectError # ����� �ʰ�.. IP:%s", inet_ntoa(Addr.sin_addr));
		closesocket(Socket);
		return FALSE;
	}

	//-----------------------------------------------------
	// ���ο� ���ǿ� ����ڸ� �ִ´�.
	//-----------------------------------------------------
	stpSession->bConnect = TRUE;

	stpSession->Socket = Socket;
	stpSession->dwSessionID = g_netSessionID;

	stpSession->clRecvQ.ClearBuffer();
	stpSession->clSendQ.ClearBuffer();

	LogOutput("Connect # IP:%s / SessionID:%d", inet_ntoa(Addr.sin_addr), g_netSessionID);

	//-----------------------------------------------------
	// ���ο� ����� ������ �˸�.
	//-----------------------------------------------------
	netPacketProc_Connect(g_netSessionID);

	g_netSessionID++;

	return TRUE;

}




////////////////////////////////////////////////////////////////////
// ��Ŷ ���� �̺�Ʈ ó��.
//
// Parameters: (int)SessionArrayCount.
// Return: ����.
////////////////////////////////////////////////////////////////////
BOOL	netRecvEvent(int iSessionArrayCount)
{
	st_NETWORK_SESSION *stpSession = &g_netSession[iSessionArrayCount];

	char				chBuff[dfNETWORK_BUFF_SIZE];
	int					iResult;
	DWORD				dwResult;


	//------------------------------------------------------------------
	// �ʴ� �ִ� ���� ���� ��Ŷ ���� 30���� ���ѵд�.
	// �̺��� ���� ��Ŷ�� �����Ѵٸ� �߸��� ����ڷ� üũ�Ѵ�.
	//------------------------------------------------------------------
	stpSession->dwTrafficCount++;
	if ( timeGetTime() > stpSession->dwTrafficTick + 1000 )
	{
		if ( dfNETWORK_MAX_RECV_SEC < stpSession->dwTrafficCount )
		{
			return FALSE;
		}
		stpSession->dwTrafficCount = 0;
	}

	//------------------------------------------------------------------
	// �ޱ� �۾�.
	//------------------------------------------------------------------
	iResult = recv(stpSession->Socket, chBuff, dfNETWORK_BUFF_SIZE, 0);

	//------------------------------------------------------------------
	// �޴ٰ� ��������.. ����� ����.
	//------------------------------------------------------------------
	if ( SOCKET_ERROR == iResult || 0 == iResult)
	{
		return FALSE;
	}

	//-------------------------------------------------------------------
	// ���� ����Ÿ�� �ִٸ�.
	//-------------------------------------------------------------------
	if ( 0 < iResult )
	{
		//-------------------------------------------------------------------
		// �ϴ� RecvStreamQ �� �ִ´�.
		//-------------------------------------------------------------------
		stpSession->clRecvQ.Put(chBuff, iResult);

		//-----------------------------------------------------------------
		// ��Ŷ�� �Ϸ� �Ǿ����� Ȯ���Ѵ�.
		//
		// ��Ŷ ó�� ��, ������ �߻��Ѵٸ� ����ó�� �Ѵ�.
		// ��Ŷ�� �ϳ� �̻��� ���ۿ� ���� �� �����Ƿ� �ѹ��� ���� ó���ؾ��Ѵ�.
		//-----------------------------------------------------------------
		while ( 1 )
		{
            dwResult = CompleteRecvPacket(stpSession->dwSessionID);

			//-------------------------------------------------------------------
			// ���̻� ó���� ��Ŷ ����.
			//-------------------------------------------------------------------
			if ( 1 == dwResult )
				break;
			
			if ( 0xff == dwResult )
				return FALSE;
		}
	}
	return TRUE;
}


	

////////////////////////////////////////////////////////////////////
// ��Ŷ �۽� �̺�Ʈ ó��.
//
// Parameters: (int)SessionArrayCount.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendEvent(int iSessionArrayCount)
{
	st_NETWORK_SESSION *stpSession = &g_netSession[iSessionArrayCount];

	DWORD	dwSendSize;

	//-----------------------------------------------------------------
	// SendQ �� �ִ� ����Ÿ���� �ִ� dfNETWORK_WSABUFF_SIZE ũ��� ������.
	//-----------------------------------------------------------------
	dwSendSize = stpSession->clSendQ.GetUseSize();
	dwSendSize = min(dfNETWORK_BUFF_SIZE, dwSendSize);

	//-----------------------------------------------------------------
	// ť�� ���� ����Ÿ�� ���� ��쿡�� �������� ����.
	//-----------------------------------------------------------------
	if ( 0 >= dwSendSize )
		return TRUE;


	char	chBuff[dfNETWORK_BUFF_SIZE];
	int		iResult;


	//-----------------------------------------------------------------
	// �ϴ� Peek �Լ��� ����Ͽ� ���� ��, 
	// ������ ����� ������ �Ǿ��� ��쿡 �� ������ �����ֵ��� ����.
	//-----------------------------------------------------------------
	stpSession->clSendQ.Peek(chBuff, dwSendSize);

	//-----------------------------------------------------------------
	// �����Ѵ�.
	//-----------------------------------------------------------------
	iResult = send(stpSession->Socket, chBuff, dwSendSize, 0);

	if ( SOCKET_ERROR == iResult )
	{
		if ( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if ( (int)dwSendSize < iResult )
		{
			//-----------------------------------------------------------------
			// ���� ���� ������� ������ �� ũ�ٸ�... ������!! �޸��ϸ� �� ���������.
			// ����� �ȵǴ� ��Ȳ������ ���� �̷� ��찡 ���涧�� �ִ�.
			//-----------------------------------------------------------------
			return FALSE;
		}
		else
		{
			//-----------------------------------------------------------------
			// �۽� �۾��� �Ϸ� �Ͽ���.
			//
			// ��Ŷ�� ������ ���۵Ǿ��ٴ°� �ƴϰ� ���� ���ۿ� ���縦 �Ϸ� �ߴٴ� �ǹ�.
			// �۽�ť ���� Peek�� ���´� �����͸� ���� ��������.
			//-----------------------------------------------------------------
			stpSession->clSendQ.RemoveData(dwSendSize);
		}
	}
	return TRUE;
}




////////////////////////////////////////////////////////////////////
// ��Ŷ ������.
//
// Parameters: (DWORD)SessionID. (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendPacket(DWORD dwSessionID, CAyaPacket *clpPacket)
{
	st_NETWORK_SESSION *stpSession = GetSession(dwSessionID);

	if ( NULL == stpSession )
		return FALSE;

	stpSession->clSendQ.Put(clpPacket->GetBufferPtr(), clpPacket->GetDataSize());

	return TRUE;
}


////////////////////////////////////////////////////////////////////
// ��ü ����ڿ��� ��Ŷ ������.
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)�Ⱥ��� ID.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendPacketAll(CAyaPacket *clpPacket, DWORD dwNoSendSessionID)
{
	int iCount;

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		//-----------------------------------------------------
		//  ���ӵǾ��ְ� ID �� �ٸ� ��쿡��...
		//-----------------------------------------------------
		if ( g_netSession[iCount].bConnect && dwNoSendSessionID != g_netSession[iCount].dwSessionID )
		{
			//-----------------------------------------------------
			// �������� ������ �ִٸ� SendQ �� ����.
			//-----------------------------------------------------
			g_netSession[iCount].clSendQ.Put(clpPacket->GetBufferPtr(), clpPacket->GetDataSize());
		}
	}

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////
// CompleteRecvPacket
//
// ��Ŷ�� �Ϸ�Ǿ����� Ȯ���� �Ϸ� �Ǿ��ٸ� ��Ŷ�� ó���Ѵ�.
//
// Parameters: (DWORD)dwSessionID.
// Return: (DWORD) 1:��Ŷó�� �Ұ� ����. 0:��Ŷó�� ����. 0xff:����.
//////////////////////////////////////////////////////////////////////////
DWORD	CompleteRecvPacket(DWORD dwSessionID)
{
	st_NETWORK_SESSION			*stpSession = GetSession(dwSessionID);
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	int		iRecvQSize;
	BYTE	byEndCode;

	iRecvQSize = stpSession->clRecvQ.GetUseSize();

	//-----------------------------------------------------------------
	// ���� ������ �˻��ؾ��Ѵ�. �׷��� ��Ŷ��� ũ�� �̻�����
	// ���� ������찡 �ƴ϶�� �˻� �غ� ������ ���� ����.. 
	//-----------------------------------------------------------------
	if ( dfNETWORK_PACKET_HEADER_SIZE > iRecvQSize )
		return 1;


	//-----------------------------------------------------------------
	// 1. PacketCode �˻�.
	//
	// Peek ���� �˻縦 �ϴ� ������, ����� ������ ������ ���Ŀ�
	// �ϳ��� �ϼ��� ��Ŷ��ŭ�� �����Ͱ� �ִ��� ������ Ȯ�� �� ��
	// ��Ŷ�� ���� ������, �׳� �ߴ������� �����ؾ� �Ѵ�.
	//
	// Get ���� ��� ���� �˻� �Ŀ� ����� �ȸ´ٸ� ����� �ٽ� ť�� 
	// ���ڸ��� �־�� �ϴµ�.. FIFO �̹Ƿ� �װ� �ȵȴ�;
	//-----------------------------------------------------------------
	stpSession->clRecvQ.Peek((char*)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	if ( dfNETWORK_PACKET_CODE != stPacketHeader.byCode ) 
		return 0xff;

	//-----------------------------------------------------------------
	// 2. ť�� ����� �����Ͱ� ����� �ϴ� ��Ŷ�� ũ�⸸ŭ �ִ��� Ȯ��.
	//    EndCode �� �����Ƿ� + 1 �� ���.
	//-----------------------------------------------------------------
	if ( stPacketHeader.bySize + dfNETWORK_PACKET_HEADER_SIZE + 1 > iRecvQSize )
	{
		//-----------------------------------------------------------------
		// ����� �۴ٸ�, ��Ŷ�� ���� �Ϸ���� �ʾ����Ƿ� ������ �ٽ� ó������.
		//-----------------------------------------------------------------
		return 1;
	}

	//-----------------------------------------------------------------
	// ������ ��� �κ��� Get �� �ƴ� Peek ���� �������ϱ�..  Q �ȿ����� ��������.
	//-----------------------------------------------------------------
	stpSession->clRecvQ.RemoveData(dfNETWORK_PACKET_HEADER_SIZE);

	CAyaPacket	clPacket;

	//-----------------------------------------------------------------
	// Payload �κ� ���۷� ����.
	//-----------------------------------------------------------------
	if ( !stpSession->clRecvQ.Get(clPacket.GetBufferPtr(), stPacketHeader.bySize) )
		return 0xff;

	//-----------------------------------------------------------------
	// EndCode �κ� ���۷� ����.
	//-----------------------------------------------------------------
	if ( !stpSession->clRecvQ.Get((char *)&byEndCode, 1) )
		return 0xff;
	if ( dfNETWORK_PACKET_END != byEndCode )
		return 0xff;


	//-----------------------------------------------------------------
	// ��Ŷ Ŭ������ ���� �����͸� ��� ���Ƿ� �����͸� �־����Ƿ�
	// Ŭ���� ������ ������ �̵�ó���� �ٽ� ������.
	//-----------------------------------------------------------------
	clPacket.MoveWritePos(stPacketHeader.bySize);


	//-----------------------------------------------------------------
	// �������� ��Ŷ ó�� �Լ��� ȣ���Ѵ�.
	//-----------------------------------------------------------------
	if ( !PacketProc(stpSession->dwSessionID, stPacketHeader.byType, &clPacket) )
		return 0xff;


	return 0;
}




//////////////////////////////////////////////////////////////////////////
// ��Ŷ Ÿ�Կ� ���� ó�� �Լ� ȣ��.
//
// Parameters: (DWORD)SessionID. (BYTE)PacketType. (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
BOOL	PacketProc(DWORD dwSessionID, BYTE byPacketType, CAyaPacket *clpPacket)
{
	switch ( byPacketType )
	{
	case dfPACKET_CS_MOVE_START:
		return netPacketProc_MoveStart(dwSessionID, clpPacket);
		break;

	case dfPACKET_CS_MOVE_STOP:
		return netPacketProc_MoveStop(dwSessionID, clpPacket);
		break;

	case dfPACKET_CS_ATTACK1:
		return netPacketProc_Attack1(dwSessionID, clpPacket);
		break;

	case dfPACKET_CS_ATTACK2:
		return netPacketProc_Attack2(dwSessionID, clpPacket);
		break;

	case dfPACKET_CS_ATTACK3:
		return netPacketProc_Attack3(dwSessionID, clpPacket);
		break;
        
	case dfPACKET_CS_SYNC:
		return netPacketProc_Sync(dwSessionID, clpPacket);
		break;
	}

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// ����� �������� ó��.
//
// Parameters: (DWORD)SessionID.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netRemoveSession(DWORD dwSessionID)
{
	st_NETWORK_SESSION	*stpSession = NULL;
		
	stpSession = GetSession(dwSessionID);

	if ( NULL == stpSession )
		return TRUE;

	//-----------------------------------------------------------------
	// �����ʿ� ����� ���Ḧ �˸���.
	//-----------------------------------------------------------------
	netPacketProc_Disconnect(stpSession->dwSessionID);


	//-----------------------------------------------------------------
	// ������ ���� ó��.
	//-----------------------------------------------------------------
	closesocket(stpSession->Socket);

	stpSession->Socket = INVALID_SOCKET;
	stpSession->bConnect = FALSE;
	stpSession->dwSessionID = 0;


	LogOutput("Disconnect # Session ID:%d", dwSessionID);

	return TRUE;
}






/////////////////////////////////////////////////////////////////////
// �ƿ�ǲ!
//
// Parameters: (char *)Format, ...
// Return: ����.
/////////////////////////////////////////////////////////////////////
void	LogOutput(char *lpszFmt, ... )
{
	char szOutText[256];
	va_list Vargs;

	//-----------------------------------------------------------------
	// �������ڸ� �ٷ� �޾� ���ڿ� �������� ..
	//-----------------------------------------------------------------
	va_start(Vargs, lpszFmt);
	vsprintf(szOutText, lpszFmt, Vargs);
	va_end(Vargs);
	strcat(szOutText, "\n");

	printf(szOutText);

}
