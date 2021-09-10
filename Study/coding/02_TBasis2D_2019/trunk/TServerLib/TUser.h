#include "TPacketPool.h"
#include "TObjectPool.h"
#define MAX_USER_BUFFER_SIZE PACKET_MAX_DATA_SIZE+ PACKET_HEADER_SIZE

struct OVERLAPPED2 : TObjectPool<OVERLAPPED2>
{
	OVERLAPPED ov;
	int iFlag;
};

class TUser : public TServerObj
{
public:
	long long 		m_dwPacketCnt;
	static SOCKET	findSock;
	WSABUF			sendBuffer;
	WSABUF			recvBuffer;
	HANDLE			userEvent;
	bool			isChatting;
	bool			bConnect;
	DWORD			m_dwEndPos;
	DWORD			m_dwStartPos;
	DWORD			m_dwReadPos;
	DWORD			m_dwWritePos;
	
	std::string		name;
	SOCKET			sock;
	SOCKADDR_IN		clientaddr;
	int				m_iRecvByte;
	char			m_szRecvBuffer[MAX_USER_BUFFER_SIZE];
	char			m_szPackStream[MAX_STREAM_SIZE];
	bool operator() (TUser& val) const
	{
		return (findSock == val.sock);
	}
	bool Dispatch(DWORD dwTransfer, OVERLAPPED2* ov);
	void Put(char* pBuffer, DWORD dwByte);
	bool WaitForReceive();
public:
	TUser();
	virtual ~TUser();
};
