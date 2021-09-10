#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TClient.h"
#include "TDebugString.h"


unsigned __stdcall sendMessage( void * arg )
{
	I_DebugStr.DisplayText("%s\r\n", "send 스레드 시작" );

	TClient* pClient = (TClient*)arg;

	while(!pClient->m_bExit)
	{		
		if( pClient->IsVerificationLogin && pClient->m_bSend )
		{
			if( pClient->SendMsg(pClient->m_strBuffer, sizeof(pClient->m_strBuffer), PACKET_CHAT_MSG ) <= 0 )
			{
				break;
			}			
		}	
		Sleep(1);
	}
	return 0;
}
unsigned __stdcall receiveMessage( void * arg )
{
	I_DebugStr.DisplayText("%s\r\n", "receive 스레드 시작" );
	TClient* pClient = (TClient*)arg;
	int iSocket = pClient->m_iSocket;
	char  strBuffer[2048]= {0,};

	while(!pClient->m_bExit)
	{		
		int iRet = recv( iSocket, strBuffer, sizeof(strBuffer)-1, 0);
		if( iRet <= 0 ) 
		{
			//비동기 소켓
			if( WSAGetLastError() == WSAEWOULDBLOCK )
			{
				continue;
			}

			pClient->m_bExit = true;
			closesocket(pClient->m_iSocket);
			break;
		}
		strBuffer[iRet] = 0;	
		EnterCriticalSection(&pClient->m_CS);
			pClient->m_StreamPacket.Put( strBuffer, iRet );
		LeaveCriticalSection(&pClient->m_CS);
		
		Sleep(1);
	}
	return 0;
}
int TClient::LoginAck(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int iRet = -1;
	int iUserIndex = -1;
	RecvStream >> iRet;
	RecvStream >> m_tGuidLogin;
	RecvStream >> m_tGuidUser;
	RecvStream >> m_tGuidActiveCharacter;
	// 기본 케릭터 생성
	if (iRet)
	{

	}
	return iRet;
}
bool TClient::Verification()
{
	stringstream data;
	data << m_tGuidLogin;
	if (SendMsg(MakePacket(PACKET_VERIFICATION_REQ, data)) == -1)
	{
		return false;
	}
	return true;
}
int TClient::VerificationAck(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	int iRet = -1;

	RecvStream >> iRet;
	if (iRet == 1)
	{
		RecvStream >> m_tGuidUser;
		RecvStream >> m_tGuidActiveCharacter;
	}
	return iRet;
}
TCharacter TClient::RecvCharacter(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	// 새 플레이어 정보를 얻어 반환
	TCharacter tChar;
	RecvStream >> tChar;
	return tChar;
}
TCharacterMap * TClient::RecvZonePlayersInfo(UPACKET& packet)
{
	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	RecvStream >> m_PlayerCharacterList;
	return &m_PlayerCharacterList;
}
bool TClient::SendRespawnPlayer(TCharacter& info)
{
	// 캐릭터 정보 직렬화
	stringstream SendStream;
	SendStream << info;
	if (SendMsg(MakePacket(PACKET_SPAWN_CHARACTER, SendStream)) == -1)	
	{
		return false;
	}
	return true;
}
void TClient::SendZoneEntry()
{
	TCharacter tChar = GetCharacterInfo();
	tChar.SetGuid(m_tGuidActiveCharacter);
	SendZoneEntryReq(tChar);
}
bool TClient::SendZoneEntryReq(TCharacter& info)
{
	stringstream SendStream;
	SendStream << info;
	if (SendMsg(MakePacket(PACKET_ZONE_ENTRY_REQ, SendStream)) == -1)
	{
		return false;
	}
	return true;
}
void TClient::RecvZoneInfo(UPACKET& packet)
{
	m_PlayerCharacterList.tCharMap.clear();
	m_GameNpcList.tCharMap.clear();

	string msg(packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	RecvStream >> m_PlayerCharacterList;
	RecvStream >> m_GameNpcList;
}
void TClient::RecvZoneEntry()
{
	SpawnPCList();
	SpawnNPCList();
	m_bGameStart = true;
}
TCharacter TClient::GetCharacterInfo()
{
	TCharacter tChar;
	if (m_PlayerCharacterList.tCharMap.size() > 0)
	{
		std::map<tGUID, TCharacter, GUIDComparer>::iterator player;
		player = m_PlayerCharacterList.tCharMap.find(m_tGuidActiveCharacter);
		if (player != m_PlayerCharacterList.tCharMap.end())
		{
			tChar = player->second;
		}
	}
	tChar.Set(  I_GameUser.GetActorLocation(),
				I_GameUser.GetActorRotation(),
				I_GameUser.GetVelocity());
	return  tChar;
}
// 존 집입시에 한번만 유저케릭터를 스폰한다.
void TClient::SpawnPCList()
{
	std::map<tGUID, TCharacter, GUIDComparer>::iterator player;
	for (player = m_PlayerCharacterList.tCharMap.begin(); player != m_PlayerCharacterList.tCharMap.end(); player++)
	{
		if (player->first == m_tGuidActiveCharacter || !player->second.GetAlive())
		{
			continue;
		}
		TCharacter& tCharacter = (*player).second;
		TGameUser SpawnCharacter;
		SpawnCharacter.m_iModelType = 1;
		SpawnCharacter.m_fPosX = tCharacter.GetActorLocation().X;
		SpawnCharacter.m_fPosY = tCharacter.GetActorLocation().Y;
		SpawnCharacter.m_IsSpawn = true;
		SpawnCharacter.m_tGuid = player->second.GetGuid();
		I_GameUser.m_UserList.push_back(SpawnCharacter);		
	}
}
void TClient::SpawnNPCList()
{
	//if (m_pTNet != nullptr)
	//{
	//	UWorld* const world = GetWorld();
	//	if (world == nullptr)
	//	{
	//		return;
	//	}

	//	for (auto& data : m_pTNet->GetNpcList().tCharMap)
	//	{
	//		TCharacter& npc = data.second;
	//		FActorSpawnParameters SpawnParams;
	//		SpawnParams.Owner = this;
	//		SpawnParams.Instigator = Instigator;
	//		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//		FGuid fGuid;
	//		tGUID tGuid = npc.GetGuid();
	//		CopyMemory(&fGuid, &tGuid, sizeof(fGuid));
	//		SpawnParams.Name = FName(*fGuid.ToString());

	//		const FVector pos = npc.GetActorLocation();
	//		ATNpc* SpawnEnemyCharacter = world->SpawnActor<ATNpc>(NpcCharacterClass,
	//			pos,
	//			FRotator::ZeroRotator,
	//			SpawnParams);
	//		if (SpawnEnemyCharacter)
	//		{
	//			SpawnEnemyCharacter->SpawnDefaultController();
	//			SpawnEnemyCharacter->m_tGuid = npc.GetGuid();
	//			SpawnEnemyCharacter->m_fHealth = npc.GetHealth();
	//			//if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, npc.GetActorLocation().ToString());
	//		}
	//	}
	//}
}
void TClient::OnPossess()
{
	TCharacter tChar = GetCharacterInfo();
	SendRespawnPlayer(tChar);
}
// 완성된 패킷에 대한 처리
int TClient::ProcessPacket()
{
	EnterCriticalSection(&m_CS);
	std::list<UPACKET>::iterator	iter;
	for( iter =  m_StreamPacket.m_PacketList.begin();
		 iter != m_StreamPacket.m_PacketList.end();
		 iter++ )
	{			
		UPACKET* pPacket = &(*iter);
		switch( pPacket->ph.type )
		{
			case PACKET_LOGIN_ACK:
			{		
				LoginAck(*pPacket);		
				IsVerificationLogin = true;
				Verification();
			}break;
			case PACKET_VERIFICATION_ACK:
			{
				if (VerificationAck(*pPacket) == 1)
				{
					if (IsVerificationLobby == false)
					{
						IsVerificationLobby = true;
						Verification();		
					}									
					else if (IsVerificationZone == false)
					{
						IsVerificationZone = true;
						OnPossess();
						SendZoneEntry();											
					}					
				}
				else
				{
					DisConnected();
				}
				break;
			}break;
			case PACKET_CREATE_CHARACTER_ACK:
			{				
			}break;
			case PACKET_CHAT_NAME_REQ:		
			{
				I_DebugStr.DisplayText("%s\r\n", pPacket->msg );
				memset(m_strBuffer, 0, sizeof(char) * 128);
				strcpy( m_strBuffer, "홍길동");
				m_bSend = true;
				while( !m_bExit )
				{								
					if( m_bSend && SendMsg( m_strBuffer,strlen(m_strBuffer),PACKET_CHAT_NAME_ACK) )
					{
						break;
					}					
				}
			}break;
			case PACKET_CHAT_MSG:	
			{
				I_DebugStr.DisplayText("%s\r\n", pPacket->msg );
			}break;
			case PACKET_USER_POSITION:
			{
				TPACKET_USER_POSITION user;			
				memcpy( &user, pPacket->msg, sizeof(char)* pPacket->ph.len - PACKET_HEADER_SIZE);
				if (I_GameUser.m_iGuid == user.user_idx)
				{
					I_GameUser.m_iDirection = user.direction;
				}
				else
				{
					I_GameUser.m_UserList[user.user_idx].m_fPosX = user.posX;
					I_GameUser.m_UserList[user.user_idx].m_fPosY = user.posY;
					I_GameUser.m_UserList[user.user_idx].m_iDirection = user.direction;
				}
			}break;
			case PACKET_STOP_POSITION:
			{
				I_GameUser.m_iDirection = 0;
			}break;
			case TPACKET_TYPE::PACKET_DEAD_CHARACTER:
			{
				RecvCharacter(*pPacket);
			}break;
			case TPACKET_TYPE::PACKET_DAMAGE_CHARACTER:
			{
				RecvCharacter(*pPacket);
			}break;
			case TPACKET_TYPE::PACKET_SPAWN_CHARACTER:
			{
				RecvCharacter(*pPacket);
				I_GameUser.m_IsSpawn = true;
			}break;
			case TPACKET_TYPE::PACKET_LOGOUT_PLAYER:
			{
				RecvCharacter(*pPacket);
			}
			break;
			case TPACKET_TYPE::PACKET_ZONE_ENTRY_ACK:
			{
				RecvZoneInfo(*pPacket);
				RecvZoneEntry();
			}break;
			case TPACKET_TYPE::PACKET_ZONE_PLAYERS_INFO:
			{
				RecvZonePlayersInfo(*pPacket);
			}
			break;			
			case TPACKET_TYPE::PACKET_ZONE_NEW_PLAYER:
			{
				RecvCharacter(*pPacket);
			}
			break;
			case TPACKET_TYPE::PACKET_SPAWN_NPC_LIST:
			{
				/*m_pTNet->RecvNpcMap(Packet);
				m_pPlayerController->SpawnNPCList();*/
			}
			break;
			case TPACKET_TYPE::PACKET_SYNC_NPC_LIST:
			{
				//m_pPlayerController->UpdateEnemyList(m_pTNet->RecvNpcMap(Packet));
			}
			break;
			case TPACKET_TYPE::PACKET_SPAWN_NPC:
			{
				//m_pPlayerController->SpawnEnemyCharacter(m_pTNet->RecvEnemyCharacter(Packet));
			}
			break;
			case TPACKET_TYPE::PACKET_DEAD_NPC:
			{
				//m_pPlayerController->DeadEnemyCharacter(m_pTNet->RecvEnemyCharacter(Packet));
			}
			break;
			case TPACKET_TYPE::PACKET_NPC_TARGETMOVE:
			{
				/*TCharacter deskChar;
				TCharacter srcChar = m_pTNet->RecvNpcMove(Packet, deskChar);
				m_pPlayerController->MoveEnemyNPC(srcChar, deskChar);*/
			}
			break;
			case TPACKET_TYPE::PACKET_ZONE_TARGET_REQ:
			{
				//SendTargetPointInfo();
			}break;
			case TPACKET_TYPE::PACKET_ATTACK_NPC:
			{
				/*TCharacter deskChar;
				TCharacter srcChar = m_pTNet->RecvNpcAttack(Packet, deskChar);
				m_pPlayerController->AttackEnemyCharacter(srcChar, deskChar);*/
			}break;
		}
	}
	m_StreamPacket.m_PacketList.clear();
	LeaveCriticalSection(&m_CS);
	return 1;
}
int TClient::SendMsg( char* pMsg, int iSize, WORD code )
{
	UPACKET sendmsg;
	ZeroMemory( &sendmsg, sizeof(sendmsg) );
	sendmsg.ph.iotype = 777;
	sendmsg.ph.type = code;
	sendmsg.ph.len = PACKET_HEADER_SIZE + iSize;

	memcpy( sendmsg.msg, pMsg, iSize );	
	int iSend = 0;
	int sendbytes = 0;

	do {
		iSend = send(m_iSocket, (char*)&sendmsg, sendmsg.ph.len, 0);
		if (iSend <= 0)
		{
			return 0;
		}
		sendbytes += iSend;
	} while (sendbytes < sendmsg.ph.len);
	m_bSend = false;		
	return 1;
}
int TClient::SendMsg(const stringstream& sendPacket)
{	
	int iSend = 0;
	int sendbytes = 0;
	do {
		iSend = send(m_iSocket, sendPacket.str().c_str(), sendPacket.str().length(), 0);
		if (iSend <= 0)
		{
			return 0;
		}
		sendbytes += iSend;
	} while (sendbytes < sendPacket.str().length());
	m_bSend = false;
	return 1;
}
int TClient::CreateConnectSocket( int iPort)
{
	int     rv;
	m_iSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_iSocket == INVALID_SOCKET )
	{
		return -1;
	}
	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	serveraddr.sin_addr.s_addr= inet_addr("127.0.0.1");
	serveraddr.sin_port		= htons(iPort);
	rv = connect(m_iSocket,(sockaddr*)&serveraddr, sizeof(serveraddr));
	if( rv == SOCKET_ERROR )
	{
		return -1;
	}	

	//struct linger
	//{
	//	int l_onoff;   //linger 옵션 사용여부
	//	int l_linger;  //linger time
	//}
	linger optval;
	int iLength = sizeof(optval);	
	getsockopt(m_iSocket,	SOL_SOCKET,	SO_LINGER,(char*)&optval, &iLength );
	optval.l_onoff  = 1;
	optval.l_linger = 1000;
	setsockopt(m_iSocket,	SOL_SOCKET,SO_LINGER,(char*)&optval, iLength );


	int socketType1;
	int typeLen1=sizeof(socketType1);
	getsockopt(m_iSocket,	SOL_SOCKET,
							SO_TYPE,
							(char*)&socketType1,&typeLen1);

	if(socketType1==SOCK_STREAM)
		I_DebugStr.DisplayText("%s\r\n", "SOCK_STREAM." );		
	else
		I_DebugStr.DisplayText("%s\r\n", "SOCK_DGRAM" );		

	getsockopt(m_iSocket,	SOL_SOCKET,
							SO_SNDBUF,
							(char*)&socketType1,&typeLen1);
	I_DebugStr.DisplayText("%s=%d\r\n","SO_SNDBUF",socketType1 );

	getsockopt(m_iSocket,	SOL_SOCKET,
							SO_RCVBUF,
							(char*)&socketType1,&typeLen1);
	I_DebugStr.DisplayText("%s=%d\r\n", "SO_RCVBUF",socketType1 );
	
	
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	int iMode = 238;
	ioctlsocket(m_iSocket, FIONBIO, (u_long FAR*) &iMode);

	
	return 0;
}
bool TClient::Init()
{
	InitializeCriticalSection(&m_CS);
	int rv;
	WSADATA wsa;
	if( WSAStartup( MAKEWORD(2,2), &wsa ) != 0 )
	{
		return -1;
	}
	
	if( CreateConnectSocket( 10000 ) == 0 )
	{		
		m_bConnectOK = true;
		// send thread		
		unsigned  sendThread, receiveThread;
		m_hSendThread = _beginthreadex( NULL, 0, sendMessage,
			(void*)this, 0, &sendThread);
		// recv thread
		m_hReceiveThread = _beginthreadex( NULL, 0, receiveMessage,
			(void*)this, 0, &receiveThread);	

		stringstream login;
		login << "kgca" << endl;
		login << "1234" << endl;
		SendMsg(MakePacket(PACKET_LOGIN_REQ, login));
	}	
	return true;
}
bool TClient::Frame()
{
	ProcessPacket();
	if( m_bExit )
	{
		I_DebugStr.DisplayText("%s\r\n", "서버가 종료되었습니다." );		
		m_bExit = false;
	}
	return true;
}
void TClient::DisConnected()
{
	closesocket( m_iSocket );
}
bool TClient::Release()
{		
	m_bExit = true;
	
	WaitForSingleObject((HANDLE)m_hSendThread, INFINITE );	
	WaitForSingleObject((HANDLE)m_hReceiveThread, INFINITE );	

	CloseHandle( (HANDLE)m_hReceiveThread );
	CloseHandle( (HANDLE)m_hSendThread );

	m_hReceiveThread	= 0;
	m_hSendThread		= 0;
	DeleteCriticalSection(&m_CS);
	
	WSACleanup();
	return true;
}
TClient::TClient(void)
{
	m_bSend		= false;
	m_bConnectOK= false;
	m_bExit		= false;
	m_bGameStart= false;
	m_hReceiveThread = 0;
	m_hSendThread = 0;
	IsVerificationLogin = false;
	IsVerificationLobby = false;
	IsVerificationZone = false;
}

TClient::~TClient(void)
{
}
