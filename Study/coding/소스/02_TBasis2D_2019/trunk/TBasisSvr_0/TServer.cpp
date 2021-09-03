#include "TServer.h"
#include "TDebugString.h"
bool TServer::Run()
{
	char	buf[2048]= {0,};
	int     iRet;

	while(1)
	{
		// 이벤트 객체 관찰
		DWORD index = WSAWaitForMultipleEvents(	m_iEventCount, 
												m_EventArray,
												FALSE, 
												WSA_INFINITE, 
												FALSE);
		if(index == WSA_WAIT_FAILED)
		{
			I_DebugStr.DisplayText("%s\r\n", "오류 : WSAWaitForMultipleEvents.");				
			continue;
		}
		//index -= WSA_WAIT_EVENT_0;
		WSAResetEvent(m_EventArray[index]);
		if( index == 0 )
		{
			continue;
		}

		TUser* pUser = GetUser(index);
		if( pUser == NULL ) break;

		DWORD cbTransferred, flags;
		iRet = WSAGetOverlappedResult(
					pUser->m_Socket,
					(LPOVERLAPPED)&pUser->m_ov, //WSAOVERLAPPED
					&cbTransferred, 
					FALSE, 
					&flags );

		if( iRet == FALSE || cbTransferred == 0)
		{
			if( iRet == FALSE ) 
				I_DebugStr.DisplayText("%s%s\r\n", pUser->m_Name.c_str(),"비동기 소켓 오류.");				
			// 클라이언트 접속 종료
			DelUser(index);
			continue;			
		}
		// 패킷처리
		if(pUser->m_ov.m_iFlags == OVERLAPPED2::MODE_RECV)
		{
			pUser->m_StreamPacket.Put(	pUser->m_wsaBuffer.buf,cbTransferred, pUser);					
			// wsasend
		}else //SEND
		{
			//......
		}		

		pUser->m_ov.m_iFlags = OVERLAPPED2::MODE_RECV;
		pUser->m_wsaBuffer.buf = pUser->m_strBuffer;
		pUser->m_wsaBuffer.len	= 2048;
		iRet = WSARecv( pUser->m_Socket,
						&(pUser->m_wsaBuffer), 1, 
						&cbTransferred, &flags, 
						(LPOVERLAPPED)&pUser->m_ov, NULL );
		if( iRet == SOCKET_ERROR )
		{
			if( WSAGetLastError() != WSA_IO_PENDING )
			{
				I_DebugStr.DisplayText("%s%s\r\n", pUser->m_Name.c_str(),"WSARecv.");							
			}
		}
		
		ProcessPacket();
		Sleep(10);
	}
	return 0;
}
void TServer::ProcessPacket()
{
	WaitForSingleObject( I_Server.m_Mutex, INFINITE );
				
	int retval;
	std::list<T_PACKET>::iterator	iter;
	for( iter =  m_StreamPacket.m_PacketList.begin();
		 iter != m_StreamPacket.m_PacketList.end();
		 iter++ )
	{		
		UPACKET PacketMsg;
		ZeroMemory( &PacketMsg, sizeof(PacketMsg) );

		T_PACKET* pSendUser = &(*iter);
		UPACKET* pPacket = &(pSendUser->packet);

		switch( pPacket->ph.type )
		{
			case PACKET_CHAT_NAME_ACK:		
			{		
				WaitForSingleObject( I_Server.m_Mutex, INFINITE );
				
				PacketMsg.ph.len = pPacket->ph.len;
				PacketMsg.ph.type = PACKET_CHAT_MSG;

				pPacket->msg[pPacket->ph.len-4] = 0;
				C_STR name = pPacket->msg;
				memcpy(&PacketMsg.msg, &pPacket->msg, pPacket->ph.len-4);
				
				char buffer[64] = "님이 입장하였습니다.";
				strcpy( &PacketMsg.msg[pPacket->ph.len-4], buffer );
				PacketMsg.ph.len += strlen(buffer);

				I_DebugStr.DisplayText("Message:%s\r\n", PacketMsg.msg );

					std::list<TUser*>::iterator	iter;
					int iClientUser = I_Server.m_UserList.size();
					for( iter =  I_Server.m_UserList.begin();
						iter != I_Server.m_UserList.end();
						iter++ )
					{
						TUser* pUser = (TUser*)*iter;	
					
						if( pUser->m_Socket != pSendUser->pUser->m_Socket )
						{
							retval = send(pUser->m_Socket, (char*)&PacketMsg, PacketMsg.ph.len, 0 );
							if(retval == SOCKET_ERROR)
							{			
								break;
							}
						}
						else
						{
							pUser->m_Name = name;
						}
					}	
				
			}break;
			case PACKET_CHAT_MSG:	
			{
				I_DebugStr.DisplayText("Message:%s\r\n", pPacket->msg );
				//WaitForSingleObject( I_Server.m_Mutex, INFINITE );
					std::list<TUser*>::iterator	iter;
					int iClientUser = I_Server.m_UserList.size();
					for( iter =  I_Server.m_UserList.begin();
						iter != I_Server.m_UserList.end();
						iter++ )
					{
						TUser* pUser = (TUser*)*iter;						
						if( pUser->m_Socket != pSendUser->pUser->m_Socket )
						{
							retval = send(pUser->m_Socket, (char*)pPacket, pPacket->ph.len, 0 );
							if(retval == SOCKET_ERROR)
							{			
								break;
							}
						}						
					}	
				//ReleaseMutex(I_Server.m_Mutex);
			}break;
		}
	}
	m_StreamPacket.m_PacketList.clear();
	ReleaseMutex(I_Server.m_Mutex);
	return;	
}
bool TServer::Init()
{	
	int iRet;
	WSADATA wsa;
	if( WSAStartup( MAKEWORD(2,2), &wsa ) != 0 )
	{
		return -1;
	}

	m_Mutex = CreateMutex( NULL, FALSE, _T("EditMutex"));
	
	// Accept 처리
	if( !m_Acceptor.Set(9000) )
	{		
		return false;
	}

	CreateThread();
	return true;
}
bool TServer::Frame()
{
	return true;
}
bool TServer::Render()
{
	return true;
}
bool TServer::Release()
{	
	WSACleanup();	
	std::list<TUser*>::iterator	iter;
	int iClientUser = m_UserList.size();
	for( iter =  m_UserList.begin();
		iter != m_UserList.end();
		iter++ )
	{
		TUser* pUser = (TUser*)*iter;						
		SAFE_DEL( pUser );					
	}		
	m_UserList.clear();
	CloseHandle( m_Mutex );
	m_Mutex = 0;
	return true;
}


TUser* TServer::GetUser( int iIndex )
{
	WaitForSingleObject( m_Mutex, INFINITE );
	std::list<TUser*>::iterator	iter;
	int iClientUser = m_UserList.size();
	for( iter =  m_UserList.begin();
		iter != m_UserList.end();
		iter++ )
	{		
		TUser* pUser = (TUser*)*iter;					
		if( pUser->m_iEvent == iIndex )
		{
			ReleaseMutex(m_Mutex);
			return pUser;			
		}
	}
	ReleaseMutex(m_Mutex);
	return NULL;
}
bool TServer::DelUser( int iIndex )
{
	// 방에서 나감
	WaitForSingleObject( I_Server.m_Mutex, INFINITE );	
	std::list<TUser*>::iterator	iter;
	std::list<TUser*>::iterator	delUser;

	for( iter =  I_Server.m_UserList.begin();
		iter != I_Server.m_UserList.end();
		iter++ )
	{			
		TUser* pUser = (TUser*)*iter;
		if( pUser->m_iEvent == iIndex )
		{
			I_DebugStr.DisplayText("%s%s\r\n", pUser->m_Name.c_str(),"님이 나가셨습니다.");
			closesocket( pUser->m_Socket );
			delUser = iter;
			break;
		}
	}		
	m_UserList.erase(delUser);
	m_iClientNumber--;
	ReleaseMutex( m_Mutex);	
	return true;
}
TServer::TServer(void)
{
	m_iClientNumber = 0;
}

TServer::~TServer(void)
{
}
