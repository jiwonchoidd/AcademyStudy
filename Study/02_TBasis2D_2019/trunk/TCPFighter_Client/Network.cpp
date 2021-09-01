
/*---------------------------------------------------------------

Ŭ���̾�Ʈ�� WSAAsyncSelect ���� �����
��Ʈ��ũ �Լ� ����.

---------------------------------------------------------------*/
#include "CommonInclude.h"

#include "Network.h"

#include "PacketDefine.h"
#include "PacketProc.h"

// =================================================================
// ��Ʈ��ũ ���� ���� ����.
//
// =================================================================
//---------------------------------------------------------------
// ���� �� ��Ŷ ���� ����.
//---------------------------------------------------------------
SOCKET			g_netSocket;
CAyaStreamSQ	g_netRecvQ;
CAyaStreamSQ	g_netSendQ;

char			g_netRecvWSABuff[dfNETWORK_WSABUFF_SIZE];
char			g_netSendWSABuff[dfNETWORK_WSABUFF_SIZE];

//---------------------------------------------------------------
// ���� ���� ���� �÷���. TRUE �� �Ǿ�߸� ���� ó���� ����.
//---------------------------------------------------------------
BOOL			g_netConnect			= FALSE;

//---------------------------------------------------------------
// ���� ������ ������ ������ �����ΰ�..
//---------------------------------------------------------------
BOOL			g_netSendPossibility	= FALSE;




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

	wVer = MAKEWORD(2,2);

	//---------------------------------------------------------------
	// ���� ����.
	//---------------------------------------------------------------
	if ( 0 != WSAStartup(wVer, &wsaData) )
		return FALSE;


	//---------------------------------------------------------------
	// TCP ���� ����.
	//---------------------------------------------------------------
	g_netSocket = socket(AF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == g_netSocket )
	{
		return FALSE;
	}

	//---------------------------------------------------------------
	// ���Ͽɼ� ����.
	// ���� ����ӵ��� ���ؼ� Nagle �˰��� ����.
	// ���۸��� ���� �����Ƿ� ��ŷ��� �þ�� �ȴ�.
	//---------------------------------------------------------------
	char cFlag = 1;
	if ( SOCKET_ERROR == setsockopt(g_netSocket, IPPROTO_TCP, TCP_NODELAY, &cFlag, sizeof(char)) )
	{
		return FALSE;
	}

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
	closesocket(g_netSocket);
	g_netSocket = INVALID_SOCKET;
	WSACleanup();

	return TRUE;
}


////////////////////////////////////////////////////////////////////
// WSAASyncSelect ���.
//
// Parameters: (HWND)������ �ڵ�. 
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSetAsyncSelect(HWND hWnd)
{
	int iResult;

	//---------------------------------------------------------------
	// ���Ͽ�, Connect, Read, Write, Close �̺�Ʈ ����.
	//---------------------------------------------------------------
	iResult = WSAAsyncSelect(g_netSocket, hWnd, dfNETWORK_UM, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	if ( SOCKET_ERROR == iResult )
	{
		return FALSE;
	}

	return TRUE;
}



////////////////////////////////////////////////////////////////////
// netConnet TCP/IP ����.
//
// Parameters: (char *) IP �ּ�.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netConnect(char *szIP)
{
	SOCKADDR_IN SockAddr;
	DWORD		dwResult;

	//---------------------------------------------------------------
	// ������ ����!!
	//---------------------------------------------------------------
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(dfNETWORK_PORT);
	SockAddr.sin_addr.s_addr = inet_addr(szIP);

    dwResult = WSAConnect(g_netSocket, (sockaddr *)&SockAddr, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL);

	if ( SOCKET_ERROR == dwResult )
	{
		//---------------------------------------------------------------
		// WSAAsyncSelect �� ����ϸ� �񵿱� �������� ����ǹǷ�
		// ���� 99% �� WSAEWOULDBLOC �� �߻��Ѵ�. �̴� ������ �ƴϰ�,
		// ���߿� ������ �Ϸ� �ǰڴٴ� �޽��� �̴�.
		//
		// �׷��Ƿ� �� ������ �ƴ� ��쿡�� �������� ������ �Ǵ��ϸ� �ȴ�.
		//---------------------------------------------------------------
		if ( WSAGetLastError() != WSAEWOULDBLOCK )
			return FALSE;
	}

   	return TRUE;
}



////////////////////////////////////////////////////////////////////
// ������ ���ν����� ��Ʈ��ũ ó�� �Լ�.
//
// Parameters: (WPARAM)wParam. (LPARAM)lParam.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netNetworkProc(WPARAM wParam, LPARAM lParam)
{
	//---------------------------------------------------------------
	// ���� �߻��� ����ó��.
	// ���� �̺�Ʈ Ȯ�� ���� ����ó���� ���� ����߸� �Ѵ�.
	//---------------------------------------------------------------
	if ( 0 != WSAGETSELECTERROR(lParam) )
	{
		return FALSE;
	}

	//---------------------------------------------------------------
	// ��Ʈ��ũ �̺�Ʈ ó��.
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
		// ��Ŷ �۽� ���� ���� �ٲ۴�.
		//---------------------------------------------------------------
		g_netSendPossibility = TRUE;

		if ( !netSendEvent() )
			return FALSE;

		return TRUE;

	}

	return TRUE;
}


	
////////////////////////////////////////////////////////////////////
// ��Ŷ ���� �̺�Ʈ ó��.
//
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netRecvEvent(void)
{
	DWORD	dwResult;
	DWORD	dwRecvSize;
	DWORD	dwFlag = 0;

	WSABUF	WSABuff;

	//------------------------------------------------------------------
	// ���� ���� ����.
	//-------------------------------------------------------------------
	WSABuff.len = dfNETWORK_WSABUFF_SIZE;
	WSABuff.buf = g_netRecvWSABuff;


	dwResult = WSARecv(g_netSocket, &WSABuff, 1, &dwRecvSize, &dwFlag, NULL, NULL);
	//------------------------------------------------------------------
	// �޴ٰ� ��������.. ����� ����.
	//------------------------------------------------------------------
	if ( SOCKET_ERROR == dwResult )
	{
		if (GetLastError() != WSAEWOULDBLOCK)
			return FALSE;
		else
			return TRUE;
	}


	//-------------------------------------------------------------------
	// ���� ����Ÿ�� �ִٸ�.
	//-------------------------------------------------------------------
	if ( 0 < dwRecvSize )
	{
		//-------------------------------------------------------------------
		// �ϴ� RecvStreamQ �� �ִ´�.
		//-------------------------------------------------------------------
		g_netRecvQ.Put(g_netRecvWSABuff, dwRecvSize);

		//-----------------------------------------------------------------
		// ��Ŷ�� �Ϸ� �Ǿ����� Ȯ���Ѵ�.
		//
		// ��Ŷ ó�� ��, ������ �߻��Ѵٸ� ����ó�� �Ѵ�.
		// ��Ŷ�� �ϳ� �̻��� ���ۿ� ���� �� �����Ƿ� �ѹ��� ���� ó���ؾ��Ѵ�.
		//-----------------------------------------------------------------
		while ( 1 )
		{
            dwResult = CompleteRecvPacket();

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
// Parameters: ����.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendEvent(void)
{
	DWORD	dwResult;
	DWORD	dwSendSize;
	DWORD	dwFlag = 0;

	WSABUF	WSABuff;

	//---------------------------------------------------------------
	// �۽��� ������ ��쿡�� �������� ����.
	// �� �Լ��� FD_WRITE �߻��� �� netSendPacket �Լ������� ȣ��ǹǷ�.
	//---------------------------------------------------------------
	if ( !g_netSendPossibility )
		return TRUE;

	//-----------------------------------------------------------------
	// g_netSendWSABuff �� ���� ����Ÿ�� ���� ��쿡�� �������� ����.
	//-----------------------------------------------------------------
	if ( 0 >= g_netSendQ.GetUseSize() )
		return TRUE;

	//-----------------------------------------------------------------
	// SendQ �� �ִ� ����Ÿ���� �ִ� dfNETWORK_WSABUFF_SIZE ũ��� ������.
	//-----------------------------------------------------------------
	dwSendSize = g_netSendQ.GetUseSize();
	dwSendSize = min(dfNETWORK_WSABUFF_SIZE, dwSendSize);

	//-----------------------------------------------------------------
	// �ϴ� Peek �Լ��� ����Ͽ� ���� ��, 
	// ������ ����� ������ �Ǿ��� ��쿡 �� ������ �����ֵ��� ����.
	//-----------------------------------------------------------------
	g_netSendQ.Peek(g_netSendWSABuff, dwSendSize);

	WSABuff.buf	= g_netSendWSABuff;
	WSABuff.len	= dwSendSize;
	//-----------------------------------------------------------------
	// �����Ѵ�.
	//-----------------------------------------------------------------
	dwResult = WSASend(g_netSocket, &WSABuff, 1, &dwSendSize, dwFlag, NULL, NULL);

	if ( SOCKET_ERROR == dwResult )
	{
		if ( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			//-----------------------------------------------------------------
			// ������ �ٷ� ���̻� ���� �� ����.
			//
			// m_bSendPossibility �� FALSE �� ���༭ FD_WRITE �� ������ ��������
			// �߰������� WSASend �� ������ �ʵ��� ����.
			//
			// ���� ���� ������ �ߴ� �޽����� ������ Peek �� �̾ұ� ������
			// ť�� ���� �����Ͱ� �����ִ� �����̴�. ���߿� �ٽ� ��������.
			//
			// ������ �ƴϹǷ� TRUE ����.
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
			g_netSendQ.RemoveData(dwSendSize);
		}
	}
	return TRUE;
}




////////////////////////////////////////////////////////////////////
// ��Ŷ ������.
//
// Parameters: (CAyaPacket *)PacketClass.
// Return: (BOOL)TRUE, FALSE.
////////////////////////////////////////////////////////////////////
BOOL	netSendPacket(CAyaPacket *clpPacket)
{
	BOOL bFlag = FALSE;

	//-----------------------------------------------------------------
	// ������ �� ���¿����� ���� �� �ִ�.
	//-----------------------------------------------------------------
	if ( !g_netConnect )
		return FALSE;

	g_netSendQ.Put(clpPacket->GetBufferPtr(), clpPacket->GetDataSize());

	//-----------------------------------------------------------------
	// ť�� �����͸� �־��ٸ� �ѹ� ��������.
	// �����⿡ ������ ���� ������ �ݰ� �������� ���� ó����
	// FD_CLOSE �߻��� ó�� �� ���̴�.
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
// ��Ŷ�� �Ϸ�Ǿ����� Ȯ���� �Ϸ� �Ǿ��ٸ� ��Ŷ�� ó���Ѵ�.
//
// Parameters: ����.
// Return: (DWORD) 1:��Ŷó�� �Ұ� ����. 0:��Ŷó�� ����. 0xff:����.
//////////////////////////////////////////////////////////////////////////
DWORD	CompleteRecvPacket(void)
{
	st_NETWORK_PACKET_HEADER	stPacketHeader;

	int		iRecvQSize;
	BYTE	byEndCode;

	iRecvQSize = g_netRecvQ.GetUseSize();

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
	g_netRecvQ.Peek((char*)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

	if ( dfNETWORK_PACKET_CODE != stPacketHeader.byCode ) 
		return 0xff;

	//-----------------------------------------------------------------
	// 2. ť�� ����� �����Ͱ� ����� �ϴ� ��Ŷ�� ũ�⸸ŭ �ִ��� Ȯ��.
	//    EndCode ���� + 1 ���.
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
	g_netRecvQ.RemoveData(dfNETWORK_PACKET_HEADER_SIZE);

	CAyaPacket	clPacket;

	//-----------------------------------------------------------------
	// Payload �κ� ���۷� ����.
	//-----------------------------------------------------------------
	if ( !g_netRecvQ.Get(clPacket.GetBufferPtr(), stPacketHeader.bySize) )
		return 0xff;

	//-----------------------------------------------------------------
	// Payload ������ EndCode �� �ִ��� Ȯ���Ѵ�.
	//-----------------------------------------------------------------
	if ( !g_netRecvQ.Get((char *)&byEndCode, 1) )
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
	if ( !PacketProc(stPacketHeader.byType, &clPacket) )
		return 0xff;
	
	
	return 0;
}




//////////////////////////////////////////////////////////////////////////
// ��Ŷ Ÿ�Կ� ���� ó�� �Լ� ȣ��.
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
