#include "TChatSvr.h"
#include "TServer.h"
#include "TServerIOCP.h"
#include "TDebugString.h"
bool TChatSvr::AddUser(SOCKET socket, SOCKADDR_IN address)
{
	{
		TSynchronize sync(this);
		TChatUser* pUser = NULL;		
		SAFE_NEW( pUser, TChatUser );
		// 유저 정보 체움.
		pUser->m_Socket = socket;	
		pUser->m_UserAddress = address;

		I_Debug.Print("IP=%s, PORT=%d", 
			inet_ntoa(address.sin_addr),
			ntohs(address.sin_port));

		if( pUser )
		{			
			I_Server.SendMsg(pUser->m_Socket,
				"환영! 대화명 입력하시오", 
				PACKET_CHAR_NAME_REQ);
			m_UserList.push_back( pUser );

			I_ServerIOCP.AddHandleToIOCP((HANDLE)socket, (ULONG_PTR)pUser);
			pUser->Create();
		}		
	}
	return true;
}
void TChatSvr::DeleteUser( SOCKET socket )
{
	{
		TSynchronize sync(this);
		std::list<TChatUser*>::iterator	iter;
		std::list<TChatUser*>::iterator	delUser;

		int iClientUser = I_Server.m_UserList.size();
		for( iter =  I_Server.m_UserList.begin();
			iter != I_Server.m_UserList.end();
			iter++ )
		{
			TChatUser* pUser = (TChatUser*)*iter;						
			if( pUser->m_Socket == socket )
			{
				char buffer[256] = {0,};
				sprintf_s( buffer, "%s%s", pUser->m_szName.c_str(),"님이 나가셨습니다." );
				I_Debug.Print("%s%s", pUser->m_szName.c_str(),"님이 나가셨습니다.");
				I_Server.Broadcastting( buffer, PACKET_CHAR_MSG, pUser->m_Socket);
				closesocket( pUser->m_Socket );
				delUser = iter;
				SAFE_DEL( pUser );
				break;
			}						
		}	
		I_Server.m_UserList.erase(delUser);
	}
};
//TChatUser* pUser
void TChatSvr::DeleteUser( TChatUser* pDeleteUser)
{
	{
		TSynchronize sync(this);
		std::list<TChatUser*>::iterator	iter;
		std::list<TChatUser*>::iterator	delUser;

		int iClientUser = I_Server.m_UserList.size();
		for( iter =  I_Server.m_UserList.begin();
			iter != I_Server.m_UserList.end();
			iter++ )
		{
			TChatUser* pUser = (TChatUser*)*iter;						
			if( pUser == pDeleteUser )
			{
				char buffer[256] = {0,};
				sprintf_s( buffer, "%s%s", pUser->m_szName.c_str(),"님이 나가셨습니다." );
				I_Debug.Print("%s%s", pUser->m_szName.c_str(),"님이 나가셨습니다.");
				I_Server.Broadcastting( buffer, PACKET_CHAR_MSG, pUser->m_Socket);
				closesocket( pUser->m_Socket );
				delUser = iter;
				SAFE_DEL( pUser );
				break;
			}						
		}	
		I_Server.m_UserList.erase(delUser);
	}
};
void TChatSvr::Broadcastting( UPACKET* pPacket, SOCKET SendSocket )
{
	{
		TSynchronize sync(this);
		std::list<TChatUser*>::iterator	iter;
		std::list<TChatUser*>::iterator	delUser;

		int iClientUser = I_Server.m_UserList.size();
		for( iter =  I_Server.m_UserList.begin();
			iter != I_Server.m_UserList.end();
			iter++ )
		{
			TChatUser* pUser = (TChatUser*)*iter;						
			//if( 0==SendSocket /*|| pUser->m_Socket != SendSocket*/ )
			{
				int iRet = send( pUser->m_Socket, (char*)pPacket, pPacket->ph.len, 0 );
				if( iRet < 0 )
				{
					I_Debug.T_ERROR(false);
					continue;
				}				
			}	
			I_Debug.Print("%s에게 %s 보냈습니다.", pUser->m_szName.c_str(), pPacket->msg);				
		}			
	}
}
void TChatSvr::Broadcastting( T_PACKET* pUserData)
{
	{
		TSynchronize sync(this);
		std::list<TChatUser*>::iterator	iter;
		std::list<TChatUser*>::iterator	delUser;

		UPACKET* pPacket = &pUserData->packet;
		DWORD dwSendByte;
		int iClientUser = I_Server.m_UserList.size();
		for( iter =  I_Server.m_UserList.begin();
			iter != I_Server.m_UserList.end();
			iter++ )
		{
			TChatUser* pUser = (TChatUser*)*iter;						
			if( pUser->m_Socket != pUserData->pUser->m_Socket )
			{
				/*pUser->m_szWsaBuf.buf = (CHAR *)pPacket;
				pUser->m_szWsaBuf.len = pPacket.ph.len;
				pUser->m_ov.iFlag = OVERLAPPEDEX::MODE_SEND;

				T_PACKET* P = (T_PACKET*)pUser->m_szWsaBuf.buf;

				int iRet = WSASend(pUser->m_Socket,
					&(pUser->m_szWsaBuf),
					1,
					&dwSendByte,
					0,
					(LPOVERLAPPED)&pUser->m_ov,
					NULL);*/

				int iRet = send( pUser->m_Socket, (char*)&pUserData->packet, pUserData->packet.ph.len, 0 );
				if( iRet < 0 )
				{
					I_Debug.T_ERROR(false);
					continue;
				}						
			}	
			I_Debug.Print("%s에게 %s님의 %s 보냈습니다.", pUser->m_szName.c_str(), 
					pUserData->pUser->m_szName.c_str(),
					pUserData->packet.msg);		
		}			
	}
};
void TChatSvr::Broadcastting( char* msg,  WORD code, SOCKET SendSocket  )
{
	{
		TSynchronize sync(this);
		std::list<TChatUser*>::iterator	iter;
		std::list<TChatUser*>::iterator	delUser;

		int iClientUser = I_Server.m_UserList.size();
		for( iter =  I_Server.m_UserList.begin();
			iter != I_Server.m_UserList.end();
			iter++ )
		{
			TChatUser* pUser = (TChatUser*)*iter;						
			if( 0==SendSocket || pUser->m_Socket != SendSocket )
			{
				if( !SendMsg( pUser->m_Socket, msg, code ) )
				{
					I_Debug.T_ERROR(false);
					continue;
				}				
			}	
			I_Debug.Print("%s%s%s", pUser->m_szName.c_str(), msg, "보냈습니다.");				
		}			
	}
};
bool TChatSvr::Init()
{	
	I_Debug.Print("%s", "서버 시작합니다.");
	TServer::Init();
	m_Acceptor.Set( 10000, NULL );	
	return true;
}
bool TChatSvr::Release()
{	
	return TServer::Release();
}
bool TChatSvr::Run()
{
	while(m_bStarted)
	{
		m_PacketPool.ProcessPacket();
		Sleep(10);
	}
	return true;
}
TChatSvr::TChatSvr(void)
{
	
}

TChatSvr::~TChatSvr(void)
{
}
