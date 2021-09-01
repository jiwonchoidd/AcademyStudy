/*---------------------------------------------------------------

	Aya Library - Packet.

	네트워크 패킷용 클래스.
	간편하게 패킷에 순서대로 데이타를 In, Out 한다.

	- 사용법.

	CAyaPacket cPacket;

	넣기.
	clPacket << 40030;	or	clPacket << iValue;	(int 넣기)
	clPacket << 3;		or	clPacket << byValue;	(BYTE 넣기)
	clPacket << 1.4;	or	clPacket << fValue;	(float 넣기)

	빼기.
	clPacket >> iValue;	(int 빼기)
	clPacket >> byValue;	(BYTE 빼기)
	clPacket >> fValue;	(float 빼기)

	!.	삽입되는 데이타 FIFO 순서로 관리된다.
		큐가 아니므로, 넣기(<<).빼기(>>) 를 혼합해서 사용하면 안된다.
		
----------------------------------------------------------------*/
#ifndef  __AYA_PACKET__
#define  __AYA_PACKET__

class CAyaPacket
{
public:
	
	/*---------------------------------------------------------------
	AyaPacket Enum.

	----------------------------------------------------------------*/
	enum enAYA_PACKET
	{
		eBUFFER_DEFAULT			= 1024		// 패킷의 기본 버퍼 사이즈.
	};

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:
	//				---------------------------------------------------------
	//				(int) 버퍼 사이즈.
	//				---------------------------------------------------------
	//				(const CAyaPacket &) 패킷의 복사를 위한 패킷 레퍼런스.
	// Return:
	//////////////////////////////////////////////////////////////////////////
			CAyaPacket();
			CAyaPacket(int iBufferSize);
			CAyaPacket(const CAyaPacket &clSrcAyaPacket);

	virtual	~CAyaPacket();

	//////////////////////////////////////////////////////////////////////////
	// 패킷 초기화.
	//
	// 메모리 할당을 여기서 하므로, 함부로 호출하면 안된다. 
	//
	// Parameters: (int)BufferSize.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Initial(int iBufferSize = eBUFFER_DEFAULT);
	//////////////////////////////////////////////////////////////////////////
	// 패킷  파괴.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Release(void);


	//////////////////////////////////////////////////////////////////////////
	// 패킷 청소.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);


	//////////////////////////////////////////////////////////////////////////
	// 버퍼 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)패킷 버퍼 사이즈 얻기.
	//////////////////////////////////////////////////////////////////////////
	int		GetBufferSize(void) { return m_iBufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 데이타 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) { return m_iDataSize; }



	//////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (char *)버퍼 포인터.
	//////////////////////////////////////////////////////////////////////////
	char	*GetBufferPtr(void) { return m_chpBuffer; }

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 Pos 이동. (음수이동은 안됨)
	// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
	//
	// Parameters: (int) 이동 사이즈.
	// Return: (int) 이동된 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);






	/* ============================================================================= */
	// 연산자 오퍼레이터.
	/* ============================================================================= */
	CAyaPacket	&operator = (CAyaPacket &clSrcAyaPacket);

	//////////////////////////////////////////////////////////////////////////
	// 넣기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CAyaPacket	&operator << (BYTE byValue);
	CAyaPacket	&operator << (char chValue);

	CAyaPacket	&operator << (short shValue);
	CAyaPacket	&operator << (WORD wValue);

	CAyaPacket	&operator << (int iValue);
	CAyaPacket	&operator << (DWORD dwValue);
	CAyaPacket	&operator << (float fValue);

	CAyaPacket	&operator << (__int64 iValue);
	CAyaPacket	&operator << (double dValue);

	//////////////////////////////////////////////////////////////////////////
	// 빼기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CAyaPacket	&operator >> (BYTE &byValue);
	CAyaPacket	&operator >> (char &chValue);

	CAyaPacket	&operator >> (short &shValue);
	CAyaPacket	&operator >> (WORD &wValue);

	CAyaPacket	&operator >> (int &iValue);
	CAyaPacket	&operator >> (DWORD &dwValue);
	CAyaPacket	&operator >> (float &fValue);

	CAyaPacket	&operator >> (__int64 &iValue);
	CAyaPacket	&operator >> (double &dValue);




	//////////////////////////////////////////////////////////////////////////
	// 데이타 얻기.
	//
	// Parameters: (char *)Dest 포인터. (int)Size.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char *chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// 데이타 삽입.
	//
	// Parameters: (char *)Src 포인터. (int)SrcSize.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char *chpSrc, int iSrcSize);



protected:





protected:

	//------------------------------------------------------------
	// 패킷버퍼 / 버퍼 사이즈.
	//------------------------------------------------------------
	char	m_chaBufferDefault[eBUFFER_DEFAULT];
	char	*m_chpBufferExpansion;

	char	*m_chpBuffer;
	int		m_iBufferSize;
	//------------------------------------------------------------
	// 패킷버퍼 시작 위치.	(본 클래스 에서는 사용하지 않지만, 확장성을 위해 사용)
	//------------------------------------------------------------
	char	*m_chpDataFieldStart;
	char	*m_chpDataFieldEnd;


	//------------------------------------------------------------
	// 버퍼의 읽을 위치, 넣을 위치.
	//------------------------------------------------------------
	char	*m_chpReadPos;
	char	*m_chpWritePos;


	//------------------------------------------------------------
	// 현재 버퍼에 사용중인 사이즈.
	//------------------------------------------------------------
	int		m_iDataSize;


};



#endif
