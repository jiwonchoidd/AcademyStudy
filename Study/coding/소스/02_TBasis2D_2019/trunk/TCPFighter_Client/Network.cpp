
/*---------------------------------------------------------------

클라이언트의 WSAAsyncSelect 모델을 사용한
네트워크 함수 모음.

---------------------------------------------------------------*/
#include "CommonInclude.h"

#include "Network.h"

#include "PacketDefine.h"
#include "PacketProc.h"

// =================================================================
// 네트워크 관련 전역 변수.
//
// =================================================================
//---------------------------------------------------------------
// 소켓 및 패킷 수신 버퍼.
//---------------------------------------------------------------
SOCKET			g_netSocket;
CAyaStreamSQ	g_netRecvQ;
CAyaStreamSQ	g_netSendQ;

char			g_netRecvWSABuff[dfNETWORK_WSABUFF_SIZE];
char			g_netSendWSABuff[dfNETWORK_WSABUFF_SIZE];

//---------------------------------------------------------------
// 접속 성공 여부 플래그. TRUE 로 되어야만 게임 처리를 하자.
//---------------------------------------------------------------
BOOL			g_netConnect			= FALSE;

//---------------------------------------------------------------
// 현재 데이터 전송이 가능한 순간인가..
//---------------------------------------------------------------
BOOL			g_netSendPossibility	= FALSE;




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

	wVer = MAKEWORD(2,2);

	//---------------------------------------------------------------
	// 윈속 시작.
	//---------------------------------------------------------------
	if ( 0 != WSAStartup(wVer, &wsaData) )
		return FALSE;


	//---------------------------------------------------------------
	// TCP 소켓 생성.
	//---------------------------------------------------------------
	g_netSocket = socket(AF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == g_netSocket )
	{
		return FALSE;
	}

	//---------------------------------------------------------------
	// 소켓옵션 설정.
	// 빠른 응답속도를 위해서 Nagle 알고리즘 중지.
	// 버퍼링을 하지 않으므로 통신량이 늘어나게 된다.
	//---------------------------------------------------------------
	char cFlag = 1;
	if ( SOCKET_ERROR == setsockopt(g_netSocket, IPPROTO_TCP, TCP_NODELAY, &cFlag, sizeof(char)) )
	{
		return FALSE;
	}

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
	closesocket(g_netSocket);
	g_netSocket = INVALID_SOCKET;
	WSACleanup();

	return TRUE;
}


////////////////////////////////////////////////////////////////////
// WSAASyncSelect 등록.
//
// Parameters: (HWND)윈도우 핸들. 
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSetAsyncSelect(HWND hWnd)
{
	int iResult;

	//---------------------------------------------------------------
	// 소켓에, Connect, Read, Write, Close 이벤트 연결.
	//---------------------------------------------------------------
	iResult = WSAAsyncSelect(g_netSocket, hWnd, dfNETWORK_UM, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	if ( SOCKET_ERROR == iResult )
	{
		return FALSE;
	}

	return TRUE;
}



////////////////////////////////////////////////////////////////////
// netConnet TCP/IP 접속.
//
// Parameters: (char *) IP 주소.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netConnect(char *szIP)
{
	SOCKADDR_IN SockAddr;
	DWORD		dwResult;

	//---------------------------------------------------------------
	// 서버에 접속!!
	//---------------------------------------------------------------
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(dfNETWORK_PORT);
	SockAddr.sin_addr.s_addr = inet_addr(szIP);

    dwResult = WSAConnect(g_netSocket, (sockaddr *)&SockAddr, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL);

	if ( SOCKET_ERROR == dwResult )
	{
		//---------------------------------------------------------------
		// WSAAsyncSelect 를 사용하면 비동기 소켓으로 변경되므로
		// 거의 99% 는 WSAEWOULDBLOC 이 발생한다. 이는 에러가 아니고,
		// 나중에 접속이 완료 되겠다는 메시지 이다.
		//
		// 그러므로 이 에러가 아닐 경우에만 실질적인 에러로 판단하면 된다.
		//---------------------------------------------------------------
		if ( WSAGetLastError() != WSAEWOULDBLOCK )
			return FALSE;
	}

   	return TRUE;
}



////////////////////////////////////////////////////////////////////
// 윈도우 프로시저의 네트워크 처리 함수.
//
// Parameters: (WPARAM)wParam. (LPARAM)lParam.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netNetworkProc(WPARAM wParam, LPARAM lParam)
{
	//---------------------------------------------------------------
	// 에러 발생시 에러처리.
	// 소켓 이벤트 확인 전에 에러처리를 먼저 해줘야만 한다.
	//---------------------------------------------------------------
	if ( 0 != WSAGETSELECTERROR(lParam) )
	{
		return FALSE;
	}

	//---------------------------------------------------------------
	// 네트워크 이벤트 처리.
	//---------------------------------------------------------------
	switch ( WSAGETSELECTEVENT(lParam) )
	{
	case FD_CONNECT:
		g_netConnect = TRUE;
		return TRUE;

	case FD_CLOSE:
		g_netConnect = FALSE;
		return FALSE;

	case FD_READ:
		if ( !netRecvEvent() )
			return FALSE;

		//return TRUE;

	case FD_WRITE:

		//---------------------------------------------------------------
		// 패킷 송신 가능 모드로 바꾼다.
		//---------------------------------------------------------------
		g_netSendPossibility = TRUE;

		if ( !netSendEvent() )
			return FALSE;

		return TRUE;

	}

	return TRUE;
}


	
////////////////////////////////////////////////////////////////////
// 패킷 수신 이벤트 처리.
//
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netRecvEvent(void)
{
	DWORD	dwResult;
	DWORD	dwRecvSize;
	DWORD	dwFlag = 0;

	WSABUF	WSABuff;

	//------------------------------------------------------------------
	// 수신 버퍼 셋팅.
	//-------------------------------------------------------------------
	WSABuff.len = dfNETWORK_WSABUFF_SIZE;
	WSABuff.buf = g_netRecvWSABuff;


	dwResult = WSARecv(g_netSocket, &WSABuff, 1, &dwRecvSize, &dwFlag, NULL, NULL);
	//------------------------------------------------------------------
	// 받다가 에러나면.. 종료다 종료.
	//------------------------------------------------------------------
	if ( SOCKET_ERROR == dwResult )
	{
		if (GetLastError() != WSAEWOULDBLOCK)
			return FALSE;
		else
			return TRUE;
	}


	//-------------------------------------------------------------------
	// 받은 데이타가 있다면.
	//-------------------------------------------------------------------
	if ( 0 < dwRecvSize )
	{
		//-------------------------------------------------------------------
		// 일단 RecvStreamQ 에 넣는다.
		//-------------------------------------------------------------------
		g_netRecvQ.Put(g_netRecvWSABuff, dwRecvSize);

		//-----------------------------------------------------------------
		// 패킷이 완료 되었는지 확인한다.
		//
		// 패킷 처리 중, 문제가 발생한다면 종료처리 한다.
		// 패킷은 하나 이상이 버퍼에 있을 수 있으므로 한번에 전부 처리해야한다.
		//-----------------------------------------------------------------
		while ( 1 )
		{
            dwResult = CompleteRecvPacket();

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
// Parameters: 없음.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendEvent(void)
{
	DWORD	dwResult;
	DWORD	dwSendSize;
	DWORD	dwFlag = 0;

	WSABUF	WSABuff;

	//---------------------------------------------------------------
	// 송신이 가능한 경우에만 보내도록 하자.
	// 본 함수는 FD_WRITE 발생시 와 netSendPacket 함수에서도 호출되므로.
	//---------------------------------------------------------------
	if ( !g_netSendPossibility )
		return TRUE;

	//-----------------------------------------------------------------
	// g_netSendWSABuff 에 보낼 데이타가 있을 경우에만 보내도록 하자.
	//-----------------------------------------------------------------
	if ( 0 >= g_netSendQ.GetUseSize() )
		return TRUE;

	//-----------------------------------------------------------------
	// SendQ 에 있는 데이타들을 최대 dfNETWORK_WSABUFF_SIZE 크기로 보낸다.
	//-----------------------------------------------------------------
	dwSendSize = g_netSendQ.GetUseSize();
	dwSendSize = min(dfNETWORK_WSABUFF_SIZE, dwSendSize);

	//-----------------------------------------------------------------
	// 일단 Peek 함수를 사용하여 빼낸 후, 
	// 전송이 제대로 마무리 되었을 경우에 이 내용을 지워주도록 하자.
	//-----------------------------------------------------------------
	g_netSendQ.Peek(g_netSendWSABuff, dwSendSize);

	WSABuff.buf	= g_netSendWSABuff;
	WSABuff.len	= dwSendSize;
	//-----------------------------------------------------------------
	// 전송한다.
	//-----------------------------------------------------------------
	dwResult = WSASend(g_netSocket, &WSABuff, 1, &dwSendSize, dwFlag, NULL, NULL);

	if ( SOCKET_ERROR == dwResult )
	{
		if ( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			//-----------------------------------------------------------------
			// 지금은 바로 더이상 보낼 수 없다.
			//
			// m_bSendPossibility 를 FALSE 로 해줘서 FD_WRITE 가 나오기 전까지는
			// 추가적으로 WSASend 를 해주지 않도록 하자.
			//
			// 또한 현재 보내려 했던 메시지는 위에서 Peek 로 뽑았기 때문에
			// 큐에 아직 데이터가 남아있는 상태이다. 나중에 다시 보내주자.
			//
			// 에러는 아니므로 TRUE 리턴.
			//-----------------------------------------------------------------
			g_netSendPossibility = FALSE;
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if ( WSABuff.len < dwSendSize )
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
			g_netSendQ.RemoveData(dwSendSize);
		}
	}
	return TRUE;
}




////////////////////////////////////////////////////////////////////
// 패킷 보내기.
//
// Parameters: (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendPacket(CAyaPacket *clpPacket)
{
	BOOL bFlag = FALSE;

	//-----------------------------------------------------------------
	// 접속이 된 상태에서만 보낼 수 있다.
	//-----------------------------------------------------------------
	if ( !g_netConnect )
		return FALSE;

	g_netSendQ.Put(clpPacket->GetBufferPtr(), clpPacket->GetDataSize());

	//-----------------------------------------------------------------
	// 큐에 데이터를 넣었다면 한번 보내보자.
	// 보내기에 실패할 경우는 소켓을 닫고 실질적인 종료 처리는
	// FD_CLOSE 발생시 처리 될 것이다.
	//-----------------------------------------------------------------
	if ( !netSendEvent() )
	{
		closesocket(g_netSocket);
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// CompleteRecvPacket
//
// 패킷이 완료되었는지 확인후 완료 되었다면 패킷을 처리한다.
//
// Parameters: 없음.
// Return: (DWORD) 1:패킷처리 할것 없음. 0:패킷처리 남음. 0xff:오류.
//////////////////////////////////////////////////////////////////////////
DWORD	CompleteRecvPacket(void)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	int		iRecvQSize;
	BYTE	byEndCode;

	iRecvQSize = g_netRecvQ.GetUseSize();

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
	g_netRecvQ.Peek((char*)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	if ( dfNETWORK_PACKET_CODE != stPacketHeader.byCode ) 
		return 0xff;

	//-----------------------------------------------------------------
	// 2. 큐에 저장된 데이터가 얻고자 하는 패킷의 크기만큼 있는지 확인.
	//    EndCode 까지 + 1 계산.
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
	g_netRecvQ.RemoveData(dfNETWORK_PACKET_HEADER_SIZE);

	CAyaPacket	clPacket;

	//-----------------------------------------------------------------
	// Payload 부분 버퍼로 빼옴.
	//-----------------------------------------------------------------
	if ( !g_netRecvQ.Get(clPacket.GetBufferPtr(), stPacketHeader.bySize) )
		return 0xff;

	//-----------------------------------------------------------------
	// Payload 다음에 EndCode 가 있는지 확인한다.
	//-----------------------------------------------------------------
	if ( !g_netRecvQ.Get((char *)&byEndCode, 1) )
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
	if ( !PacketProc(stPacketHeader.byType, &clPacket) )
		return 0xff;
	
	
	return 0;
}




//////////////////////////////////////////////////////////////////////////
// 패킷 타입에 따른 처리 함수 호출.
//
// Parameters: (BYTE)PacketType. (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
BOOL	PacketProc(BYTE byPacketType, CAyaPacket *clpPacket)
{
	switch ( byPacketType )
	{
	case dfPACKET_SC_CREATE_MY_CHARACTER:
		netPacketProc_CreateMyCharacter(clpPacket);
		break;

	case dfPACKET_SC_CREATE_OTHER_CHARACTER:
		netPacketProc_CreateOtherCharacter(clpPacket);
		break;

	case dfPACKET_SC_DELETE_CHARACTER:
		netPacketProc_DeleteCharacter(clpPacket);
		break;

	case dfPACKET_SC_MOVE_START:
		netPacketProc_MoveStart(clpPacket);
		break;

	case dfPACKET_SC_MOVE_STOP:
		netPacketProc_MoveStop(clpPacket);
		break;

	case dfPACKET_SC_ATTACK1:
		netPacketProc_Attack1(clpPacket);
		break;

	case dfPACKET_SC_ATTACK2:
		netPacketProc_Attack2(clpPacket);
		break;

	case dfPACKET_SC_ATTACK3:
		netPacketProc_Attack3(clpPacket);
		break;
        
	case dfPACKET_SC_DAMAGE:
		netPacketProc_Damage(clpPacket);
		break;
        


	case dfPACKET_SC_SYNC:
		netPacketProc_Sync(clpPacket);
		break;
	}

	return TRUE;
}
