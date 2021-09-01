
/*---------------------------------------------------------------

select 모델의 서버 네트워크 관련 함수.

블럭모드의 소켓을 사용한다.

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
// 네트워크 관련 전역 변수.
//
// =================================================================
//---------------------------------------------------------------
// 소켓 및 패킷 수신 버퍼.
//---------------------------------------------------------------
SOCKET				g_netListenSocket;

//---------------------------------------------------------------
// 접속자 정보 배열.
//---------------------------------------------------------------
st_NETWORK_SESSION	g_netSession[dfNETWORK_MAX_USER];

//---------------------------------------------------------------
// 접속자 ID 할당.
//---------------------------------------------------------------
DWORD				g_netSessionID = 1;



////////////////////////////////////////////////////////////////////
// 비어있는 세션 찾기.
//
// Parameters: 없음.
// Return: (st_NETWORK_SESSION *)세션 구조체.
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
// ID 로 세션 찾기.
//
// Parameters: (DWORD)SessionID.
// Return: (st_NETWORK_SESSION *)세션 구조체.
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
// Socket 으로 세션 찾기.
//
// Parameters: (SOCKET)Socket.
// Return: (st_NETWORK_SESSION *)세션 구조체.
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
// WinSock 초기화.
//
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netStartUp(void)
{
	WORD		wVer;
	WSADATA		wsaData;
	int			iCount;

	wVer = MAKEWORD(2,2);

	//---------------------------------------------------------------
	// 윈속 시작.
	//---------------------------------------------------------------
	if ( 0 != WSAStartup(wVer, &wsaData) )
		return FALSE;


	//---------------------------------------------------------------
	// TCP 소켓 생성.
	//---------------------------------------------------------------
	g_netListenSocket = socket(AF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == g_netListenSocket )
	{
		return FALSE;
	}

	//---------------------------------------------------------------
	// 소켓옵션 설정.
	// 빠른 응답속도를 위해서 Nagle 알고리즘 중지.
	// 버퍼링을 하지 않으므로 통신량이 늘어나게 된다.
	//---------------------------------------------------------------
	char cFlag = 1;
	if ( SOCKET_ERROR == setsockopt(g_netListenSocket, IPPROTO_TCP, TCP_NODELAY, &cFlag, sizeof(char)) )
	{
		return FALSE;
	}


	//---------------------------------------------------------------
	// 세션에 대한 정보 초기화.
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
// WinSock 정리
//
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netCleanUp(void)
{
	int iCount;

	closesocket(g_netListenSocket);
	g_netListenSocket = INVALID_SOCKET;

	//---------------------------------------------------------------
	// 모든 세션에 대해 클로즈 소켓 호출.
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
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netPortOpen(void)
{
	//-----------------------------------------------------
	// 어떤 IP, 어떤 포트와 연결할지 바인딩을 위해 주소값 셋팅.
	//-----------------------------------------------------
	SOCKADDR_IN Addr;
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(dfNETWORK_PORT);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//-----------------------------------------------------
	// 포트3000 번과 연결한다.
	//-----------------------------------------------------
	if ( SOCKET_ERROR == bind(g_netListenSocket, (SOCKADDR *)&Addr, sizeof(SOCKADDR_IN)) )
		return FALSE;

	LogOutput("Bind OK # Port:%d", dfNETWORK_PORT);

	//-----------------------------------------------------
	// 포트 오픈 !!
	//-----------------------------------------------------
	if ( SOCKET_ERROR == listen(g_netListenSocket, SOMAXCONN) )
		return FALSE;


	LogOutput("Listen OK #");

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// 네트워크 메인 처리.
//
// Parameters: 없음.
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
	// select 의 Recv, Send 체크를 위한 셋팅 과정.
	//-----------------------------------------------------
	FD_ZERO(&RecvSet);
	FD_ZERO(&SendSet);
	FD_SET(g_netListenSocket, &RecvSet);

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		if ( g_netSession[iCount].bConnect )
		{
			//-----------------------------------------------------
			// 접속중인 세션이 있다면 Recv 검사하자.
			//-----------------------------------------------------
			FD_SET(g_netSession[iCount].Socket, &RecvSet);

			//-----------------------------------------------------
			// 해당 세션에 SendQ 에 데이터가 있다면
			// SendSet 에도 등록을 해주자.
			//
			// Send 가능여부를 받아서 Send 작업을 해줘야 하므로.
			//-----------------------------------------------------
			if ( 0 < g_netSession[iCount].clSendQ.GetUseSize() )
				FD_SET(g_netSession[iCount].Socket, &SendSet);
		}
	}

	//-----------------------------------------------------
	// Select 대기시간 설정. 
	//-----------------------------------------------------
	Time.tv_sec = 0;
	Time.tv_usec = 1;

	//-----------------------------------------------------
	// select 함수를 호출하여 검사.
	//-----------------------------------------------------
	iResult = select(0, &RecvSet, &SendSet, 0, &Time);

	//-----------------------------------------------------
	// 리턴값이 0 보다 크므로, 누군가 반응이 왔다.  찾자.
	//-----------------------------------------------------
	if ( 0 < iResult )
	{
		//-----------------------------------------------------
		// 접속요청 처리.
		//-----------------------------------------------------
		if ( FD_ISSET(g_netListenSocket, &RecvSet) )
		{
			netAcceptEvent();
		}

		//-----------------------------------------------------
		// 기존 세션의 소켓에서 온 반응을 검사하자.
		//-----------------------------------------------------
		for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
		{
			if ( g_netSession[iCount].bConnect )
			{
				//-----------------------------------------------------
				// Recv 반응이 있는지...
				//-----------------------------------------------------
				if ( FD_ISSET(g_netSession[iCount].Socket, &RecvSet) )
				{
					if ( !netRecvEvent(iCount) )
					{
						netRemoveSession(g_netSession[iCount].dwSessionID);
					}
				}
				//-----------------------------------------------------
				// Send 반응이 있는지...
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
		// 에러, 끝내자. 
		//-----------------------------------------------------
		return FALSE;
	}

	return TRUE;
}




////////////////////////////////////////////////////////////////////
// 사용자 접속 이벤트 처리.
//
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netAcceptEvent(void)
{
	SOCKADDR_IN	Addr;
	SOCKET		Socket;
	int			iSize = sizeof(Addr);

	st_NETWORK_SESSION *stpSession;

	//-----------------------------------------------------
	// 일단 접속자의 연결을 받아들인다.
	//-----------------------------------------------------
	Socket = accept(g_netListenSocket, (SOCKADDR *)&Addr, &iSize);
	
	if ( INVALID_SOCKET == Socket )
		return FALSE;

	//-----------------------------------------------------
	// 비어있는 클라이언트 위치를 찾자.
	//-----------------------------------------------------
	stpSession = GetEmptySession();

	if ( NULL == stpSession )
	{
		LogOutput("ConnectError # 사용자 초과.. IP:%s", inet_ntoa(Addr.sin_addr));
		closesocket(Socket);
		return FALSE;
	}

	//-----------------------------------------------------
	// 새로운 세션에 사용자를 넣는다.
	//-----------------------------------------------------
	stpSession->bConnect = TRUE;

	stpSession->Socket = Socket;
	stpSession->dwSessionID = g_netSessionID;

	stpSession->clRecvQ.ClearBuffer();
	stpSession->clSendQ.ClearBuffer();

	LogOutput("Connect # IP:%s / SessionID:%d", inet_ntoa(Addr.sin_addr), g_netSessionID);

	//-----------------------------------------------------
	// 새로운 사용자 접속을 알림.
	//-----------------------------------------------------
	netPacketProc_Connect(g_netSessionID);

	g_netSessionID++;

	return TRUE;

}




////////////////////////////////////////////////////////////////////
// 패킷 수신 이벤트 처리.
//
// Parameters: (int)SessionArrayCount.
// Return: 없음.
////////////////////////////////////////////////////////////////////
BOOL	netRecvEvent(int iSessionArrayCount)
{
	st_NETWORK_SESSION *stpSession = &g_netSession[iSessionArrayCount];

	char				chBuff[dfNETWORK_BUFF_SIZE];
	int					iResult;
	DWORD				dwResult;


	//------------------------------------------------------------------
	// 초당 최대 수신 가능 패킷 수를 30개로 제한둔다.
	// 이보다 많은 패킷을 수신한다면 잘못된 사용자로 체크한다.
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
	// 받기 작업.
	//------------------------------------------------------------------
	iResult = recv(stpSession->Socket, chBuff, dfNETWORK_BUFF_SIZE, 0);

	//------------------------------------------------------------------
	// 받다가 에러나면.. 종료다 종료.
	//------------------------------------------------------------------
	if ( SOCKET_ERROR == iResult || 0 == iResult)
	{
		return FALSE;
	}

	//-------------------------------------------------------------------
	// 받은 데이타가 있다면.
	//-------------------------------------------------------------------
	if ( 0 < iResult )
	{
		//-------------------------------------------------------------------
		// 일단 RecvStreamQ 에 넣는다.
		//-------------------------------------------------------------------
		stpSession->clRecvQ.Put(chBuff, iResult);

		//-----------------------------------------------------------------
		// 패킷이 완료 되었는지 확인한다.
		//
		// 패킷 처리 중, 문제가 발생한다면 종료처리 한다.
		// 패킷은 하나 이상이 버퍼에 있을 수 있으므로 한번에 전부 처리해야한다.
		//-----------------------------------------------------------------
		while ( 1 )
		{
            dwResult = CompleteRecvPacket(stpSession->dwSessionID);

			//-------------------------------------------------------------------
			// 더이상 처리할 패킷 없음.
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
// 패킷 송신 이벤트 처리.
//
// Parameters: (int)SessionArrayCount.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendEvent(int iSessionArrayCount)
{
	st_NETWORK_SESSION *stpSession = &g_netSession[iSessionArrayCount];

	DWORD	dwSendSize;

	//-----------------------------------------------------------------
	// SendQ 에 있는 데이타들을 최대 dfNETWORK_WSABUFF_SIZE 크기로 보낸다.
	//-----------------------------------------------------------------
	dwSendSize = stpSession->clSendQ.GetUseSize();
	dwSendSize = min(dfNETWORK_BUFF_SIZE, dwSendSize);

	//-----------------------------------------------------------------
	// 큐에 보낼 데이타가 있을 경우에만 보내도록 하자.
	//-----------------------------------------------------------------
	if ( 0 >= dwSendSize )
		return TRUE;


	char	chBuff[dfNETWORK_BUFF_SIZE];
	int		iResult;


	//-----------------------------------------------------------------
	// 일단 Peek 함수를 사용하여 빼낸 후, 
	// 전송이 제대로 마무리 되었을 경우에 이 내용을 지워주도록 하자.
	//-----------------------------------------------------------------
	stpSession->clSendQ.Peek(chBuff, dwSendSize);

	//-----------------------------------------------------------------
	// 전송한다.
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
			// 보낸 보낼 사이즈보다 오히려 더 크다면... 오류다!! 왼만하면 다 끊어버리자.
			// 생기면 안되는 상황이지만 가끔 이런 경우가 생길때가 있다.
			//-----------------------------------------------------------------
			return FALSE;
		}
		else
		{
			//-----------------------------------------------------------------
			// 송신 작업을 완료 하였다.
			//
			// 패킷이 완전히 전송되었다는건 아니고 소켓 버퍼에 복사를 완료 했다는 의미.
			// 송신큐 에서 Peek로 빼냈던 데이터를 이제 지워주자.
			//-----------------------------------------------------------------
			stpSession->clSendQ.RemoveData(dwSendSize);
		}
	}
	return TRUE;
}




////////////////////////////////////////////////////////////////////
// 패킷 보내기.
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
// 전체 사용자에게 패킷 보내기.
//
// Parameters: (CAyaPacket *)PacketClass. (DWORD)안보낼 ID.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendPacketAll(CAyaPacket *clpPacket, DWORD dwNoSendSessionID)
{
	int iCount;

	for ( iCount = 0; iCount < dfNETWORK_MAX_USER; iCount++ )
	{
		//-----------------------------------------------------
		//  접속되어있고 ID 가 다른 경우에만...
		//-----------------------------------------------------
		if ( g_netSession[iCount].bConnect && dwNoSendSessionID != g_netSession[iCount].dwSessionID )
		{
			//-----------------------------------------------------
			// 접속중인 세션이 있다면 SendQ 에 넣자.
			//-----------------------------------------------------
			g_netSession[iCount].clSendQ.Put(clpPacket->GetBufferPtr(), clpPacket->GetDataSize());
		}
	}

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////
// CompleteRecvPacket
//
// 패킷이 완료되었는지 확인후 완료 되었다면 패킷을 처리한다.
//
// Parameters: (DWORD)dwSessionID.
// Return: (DWORD) 1:패킷처리 할것 없음. 0:패킷처리 남음. 0xff:오류.
//////////////////////////////////////////////////////////////////////////
DWORD	CompleteRecvPacket(DWORD dwSessionID)
{
	st_NETWORK_SESSION			*stpSession = GetSession(dwSessionID);
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	int		iRecvQSize;
	BYTE	byEndCode;

	iRecvQSize = stpSession->clRecvQ.GetUseSize();

	//-----------------------------------------------------------------
	// 받은 내용을 검사해야한다. 그런데 패킷헤더 크기 이상으로
	// 뭔가 받은경우가 아니라면 검사 해볼 껀덕지 조차 없다.. 
	//-----------------------------------------------------------------
	if ( dfNETWORK_PACKET_HEADER_SIZE > iRecvQSize )
		return 1;


	//-----------------------------------------------------------------
	// 1. PacketCode 검사.
	//
	// Peek 으로 검사를 하는 이유는, 헤더를 얻은후 사이즈 비교후에
	// 하나의 완성된 패킷만큼의 데이터가 있는지 없는지 확인 한 후
	// 패킷을 마저 얻을지, 그냥 중단할지를 결정해야 한다.
	//
	// Get 으로 얻는 경우는 검사 후에 사이즈가 안맞다면 헤더를 다시 큐의 
	// 제자리에 넣어야 하는데.. FIFO 이므로 그게 안된다;
	//-----------------------------------------------------------------
	stpSession->clRecvQ.Peek((char*)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	if ( dfNETWORK_PACKET_CODE != stPacketHeader.byCode ) 
		return 0xff;

	//-----------------------------------------------------------------
	// 2. 큐에 저장된 데이터가 얻고자 하는 패킷의 크기만큼 있는지 확인.
	//    EndCode 가 있으므로 + 1 로 계산.
	//-----------------------------------------------------------------
	if ( stPacketHeader.bySize + dfNETWORK_PACKET_HEADER_SIZE + 1 > iRecvQSize )
	{
		//-----------------------------------------------------------------
		// 사이즈가 작다면, 패킷이 아직 완료되지 않았으므로 다음에 다시 처리하자.
		//-----------------------------------------------------------------
		return 1;
	}

	//-----------------------------------------------------------------
	// 위에서 헤더 부분은 Get 이 아닌 Peek 으로 빼왔으니까..  Q 안에서는 지워주자.
	//-----------------------------------------------------------------
	stpSession->clRecvQ.RemoveData(dfNETWORK_PACKET_HEADER_SIZE);

	CAyaPacket	clPacket;

	//-----------------------------------------------------------------
	// Payload 부분 버퍼로 빼옴.
	//-----------------------------------------------------------------
	if ( !stpSession->clRecvQ.Get(clPacket.GetBufferPtr(), stPacketHeader.bySize) )
		return 0xff;

	//-----------------------------------------------------------------
	// EndCode 부분 버퍼로 빼옴.
	//-----------------------------------------------------------------
	if ( !stpSession->clRecvQ.Get((char *)&byEndCode, 1) )
		return 0xff;
	if ( dfNETWORK_PACKET_END != byEndCode )
		return 0xff;


	//-----------------------------------------------------------------
	// 패킷 클래스에 버퍼 포인터를 얻어 임의로 데이터를 넣었으므로
	// 클래스 내부의 사이즈 이동처리를 다시 해주자.
	//-----------------------------------------------------------------
	clPacket.MoveWritePos(stPacketHeader.bySize);


	//-----------------------------------------------------------------
	// 실질적인 패킷 처리 함수를 호출한다.
	//-----------------------------------------------------------------
	if ( !PacketProc(stpSession->dwSessionID, stPacketHeader.byType, &clPacket) )
		return 0xff;


	return 0;
}




//////////////////////////////////////////////////////////////////////////
// 패킷 타입에 따른 처리 함수 호출.
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
// 사용자 접속해지 처리.
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
	// 게임쪽에 사용자 종료를 알린다.
	//-----------------------------------------------------------------
	netPacketProc_Disconnect(stpSession->dwSessionID);


	//-----------------------------------------------------------------
	// 세션측 종료 처리.
	//-----------------------------------------------------------------
	closesocket(stpSession->Socket);

	stpSession->Socket = INVALID_SOCKET;
	stpSession->bConnect = FALSE;
	stpSession->dwSessionID = 0;


	LogOutput("Disconnect # Session ID:%d", dwSessionID);

	return TRUE;
}






/////////////////////////////////////////////////////////////////////
// 아웃풋!
//
// Parameters: (char *)Format, ...
// Return: 없음.
/////////////////////////////////////////////////////////////////////
void	LogOutput(char *lpszFmt, ... )
{
	char szOutText[256];
	va_list Vargs;

	//-----------------------------------------------------------------
	// 가변인자를 바로 받아 문자열 형식으로 ..
	//-----------------------------------------------------------------
	va_start(Vargs, lpszFmt);
	vsprintf(szOutText, lpszFmt, Vargs);
	va_end(Vargs);
	strcat(szOutText, "\n");

	printf(szOutText);

}
