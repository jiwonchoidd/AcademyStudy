#include "TStdSelect.h"
#include "TNetwork.h"

bool		TStdSelect::Init()
{	
	return true;
}

bool		TStdSelect::Frame()
{
	FD_ZERO(&rSet);
	FD_ZERO(&wSet);
	FD_SET(m_Sock, &rSet);
	FD_SET(m_Sock, &wSet);

	timeval time;
	time.tv_sec = 0; // 초
	time.tv_usec = 1000;// 마이크로 초 1/1000000초

	//성공 시		반환 값 : 준비된 fd 개수
	//시간 만료시	반환 값 : 0
	//오류 시		반환 값 : -1		
	int iRet = select(0, &rSet, &wSet, NULL, &time);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	if (iRet == 0) return true;// timeout
		
	if (FD_ISSET(m_Sock, &rSet))
	{
		if (RecvData() == false)
		{
			TNetwork::m_bConnect = false;
		}
	}
	if (FD_ISSET(m_Sock, &wSet))
	{
		//printf(".");
	}
	return true;
}

TStdSelect::TStdSelect(SOCKET sock)
{
	m_Sock = sock;
}


TStdSelect::~TStdSelect()
{
}
