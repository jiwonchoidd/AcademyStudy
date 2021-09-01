#include "TAcceptor.h"

HANDLE		m_Mutex;
int			clientNumber=0;
int			allClientSocket[100];

unsigned __stdcall broadcastAllClient(void *arg)
{
	char fromClient[2048] = {0,};
	int myClientSocket=(int)arg;
	int i=0;
	while(1)
	{
		int readlen=recv(myClientSocket,fromClient,sizeof(fromClient),0);
		if(readlen ==INVALID_SOCKET )
		{
			break;
		}
		if(readlen<=0) 
		{
			break;
		}
		fromClient[readlen] = 0;
		  
	
		printf("%s\n",fromClient);	
		WaitForSingleObject(m_Mutex,INFINITE);//뮤텍스 동기 화 시작
			for( i=0;i<clientNumber;i++)
			{				 
				if(allClientSocket[i] != myClientSocket)
				{				
					send(allClientSocket[i],fromClient,readlen,0);				     
				}
			}
		ReleaseMutex(m_Mutex);//뮤텍스 동기화 해제	  
	} 
	WaitForSingleObject(m_Mutex,INFINITE);//뮤텍스 동기 화 시작
		ZeroMemory(fromClient, sizeof(fromClient));
		sprintf(fromClient,"%d 님이 방을 나갔습니다.",myClientSocket );
		for( i=0;i<clientNumber;i++)
		{				 
			if(allClientSocket[i] != myClientSocket)
			{				
				send(allClientSocket[i],fromClient,sizeof(fromClient),0);				     
			}
		}
	ReleaseMutex(m_Mutex);//뮤텍스 동기화 해제	  

	WaitForSingleObject(m_Mutex,INFINITE);//뮤텍스 동기화 시작
	for(i=0;i<clientNumber;i++)
	{		
		if(allClientSocket[i]==myClientSocket)
		{	
			for(;i<clientNumber-1;i++)
			{
				 allClientSocket[i]=allClientSocket[i+1];
			}
			break;
		}
		clientNumber--;
		ReleaseMutex(m_Mutex);
		closesocket(myClientSocket);
	}
	return 0;
}


int TAcceptor::GetIPList(vector<string>& ipList)
{
	char szHostName[1024];
	string strHostName;
	string strIPAddr = "";
	int  i;
	WSADATA WsaData;
	if(WSAStartup(0x202, &WsaData) == SOCKET_ERROR)
	{ 
		return 0;
	}
	if (! ::gethostname(szHostName, 1024))
	{
		strHostName = szHostName;
		struct hostent* pHost;
		pHost = gethostbyname(szHostName);
		
		if (pHost != NULL)
		{
			
			for(i=0;i<255;i++)
			{
				if (pHost->h_addr_list[i])
				{
					BYTE    bAddr[4];
					char    strIPAddr[256] = {0,};
					bAddr[0] = (BYTE) pHost->h_addr_list[i][0];
					bAddr[1] = (BYTE) pHost->h_addr_list[i][1];
					bAddr[2] = (BYTE) pHost->h_addr_list[i][2];
					bAddr[3] = (BYTE) pHost->h_addr_list[i][3];
					sprintf(strIPAddr, "%d.%d.%d.%d", bAddr[0], bAddr[1], bAddr[2], bAddr[3]);
					ipList.push_back(strIPAddr);
				}
				else
				{
					break;
				}
			}
		}
	}
	WSACleanup();
	return ipList.size();
}
bool TAcceptor::Set( int iPort, const char* strAddress )
{
	m_Mutex=CreateMutex(NULL,FALSE,NULL);
	int     iRet;
	m_ListenSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_ListenSock == INVALID_SOCKET )
	{
		return -1;
	}

	int optval = 1;
	setsockopt( m_ListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	if( strAddress == NULL)
	{
		serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);
	}
	else
	{
		serveraddr.sin_addr.s_addr= inet_addr(strAddress);
	}
	serveraddr.sin_port		= htons(iPort);
	iRet = bind(m_ListenSock, (SOCKADDR*)&serveraddr, sizeof(serveraddr) );
	if( iRet == SOCKET_ERROR )
	{
		return -1;
	}

	iRet = listen( m_ListenSock, SOMAXCONN );
	if( iRet == SOCKET_ERROR )
	{
		return -1;
	}

	CreateThread();
	return true;
}
bool TAcceptor::Run()
{
	int iRet;
	SOCKADDR_IN clientaddr;
	int			addrlen;
	SOCKET		clientSocket;
	unsigned int			threadID;
	while(m_bStarted)
	{
		clientSocket = 10000;
		addrlen = sizeof( clientaddr );
		clientSocket = accept( m_ListenSock, (SOCKADDR*)&clientaddr, &addrlen );
		if( clientSocket == INVALID_SOCKET )
		{
			continue;
		}	
		
		WaitForSingleObject(m_Mutex,INFINITE);	
			clientNumber++;	
			allClientSocket[clientNumber-1]=clientSocket;	
		ReleaseMutex(m_Mutex); 	

		char greetMessage[256]={0,};	
		sprintf(greetMessage,"[서버]환영합니다. 대화명을 입력해 주세요.");	
		send(clientSocket,greetMessage,sizeof(greetMessage),0);	
		unsigned long thread;
		thread=_beginthreadex(NULL,0,broadcastAllClient,(void*)clientSocket,0,&threadID);			
		
		Sleep(1);
	}
	m_bStarted = false;	
	return 0;
}
bool TAcceptor::Release()
{
	CloseHandle((HANDLE)m_hThread);
	m_bStarted = false;
	return true;
}
TAcceptor::TAcceptor(void)
{
	clientNumber = 0;
}

TAcceptor::~TAcceptor(void)
{
}
