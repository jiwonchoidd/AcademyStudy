

/*---------------------------------------------------------------

	Aya Library - Packet.

	네트워크 패킷용 클래스.
	간편하게 패킷에 순서대로 데이타를 In, Out 한다.

	- 사용법.

	CAyaPacket clPacket;

	넣기.
	clPacket << 40030;	or	clPacket << iValue;	(int 넣기)
	clPacket << 3;		or	clPacket << byValue;	(BYTE 넣기)
	clPacket << 1.4;		or	clPacket << fValue;	(float 넣기)

	빼기.
	clPacket >> iValue;	(int 빼기)
	clPacket >> byValue;	(BYTE 빼기)
	clPacket >> fValue;	(float 빼기)

	!.	삽입되는 데이타 FIFO 순서로 관리된다.
		큐가 아니므로, 넣기(<<).빼기(>>) 를 혼합해서 사용하면 안된다.
		
----------------------------------------------------------------*/
#include <windows.h>
#include <assert.h>
#include "AyaPacket.h"


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
CAyaPacket::CAyaPacket() :	m_chpBuffer(NULL),
							m_chpDataFieldStart(NULL),
							m_chpDataFieldEnd(NULL),
							m_chpReadPos(NULL),
							m_chpWritePos(NULL)
{
	/*-----------------------------------------------------------------
	버퍼 사이즈를 입력하지 않는다면, 기본치로 생성.
	-------------------------------------------------------------------*/
	m_chpBuffer = NULL;
	m_chpBufferExpansion = NULL;
	Initial();
}


CAyaPacket::CAyaPacket(int iBufferSize) :	m_chpBuffer(NULL),
											m_chpDataFieldStart(NULL),
											m_chpDataFieldEnd(NULL),
											m_chpReadPos(NULL),
											m_chpWritePos(NULL)
{
	m_chpBuffer = NULL;
	m_chpBufferExpansion = NULL;
	Initial(iBufferSize);
}

CAyaPacket::CAyaPacket(const CAyaPacket &cSrcAyaPacket) :	m_chpBuffer(NULL),
															m_iBufferSize(0),
															m_chpDataFieldStart(NULL),
															m_chpDataFieldEnd(NULL),
															m_chpReadPos(NULL),
															m_chpWritePos(NULL)
{
	m_chpBuffer = NULL;
	m_chpBufferExpansion = NULL;
	Initial(cSrcAyaPacket.m_iBufferSize);
	
	/*-----------------------------------------------------------------
	원본 패킷 클래스에서 복사해온다.
	-------------------------------------------------------------------*/
	PutData(cSrcAyaPacket.m_chpReadPos, cSrcAyaPacket.m_iDataSize);
	
}


CAyaPacket::~CAyaPacket()
{
	Release();
}






//////////////////////////////////////////////////////////////////////////
// 패킷 초기화.
//
// Parameters: (int)BufferSize.
// Return: 없음.
//////////////////////////////////////////////////////////////////////////
void CAyaPacket::Initial(int iBufferSize)
{
	m_iBufferSize = iBufferSize;

	if ( NULL == m_chpBuffer )
	{
		if ( eBUFFER_DEFAULT < m_iBufferSize )
		{
			m_chpBufferExpansion = new char[m_iBufferSize];
			m_chpBuffer = m_chpBufferExpansion;
		}
		else
		{
			m_chpBufferExpansion = NULL;
			m_chpBuffer = m_chaBufferDefault;
		}
	}

	assert(m_chpBuffer);

	m_chpDataFieldStart = m_chpBuffer;
	m_chpDataFieldEnd = m_chpBuffer + m_iBufferSize;

	m_chpReadPos = m_chpWritePos = m_chpDataFieldStart;

	m_iDataSize = 0;
}


//////////////////////////////////////////////////////////////////////////
// 패킷  파괴.
//
// Parameters: 없음.
// Return: 없음.
//////////////////////////////////////////////////////////////////////////
void CAyaPacket::Release(void)
{
	if ( NULL != m_chpBufferExpansion )
		delete[] m_chpBufferExpansion;
}


//////////////////////////////////////////////////////////////////////////
// 패킷 청소.
//
// Parameters: 없음.
// Return: 없음.
//////////////////////////////////////////////////////////////////////////
void CAyaPacket::Clear(void)
{
	assert(m_chpBuffer);

	m_chpDataFieldStart = m_chpBuffer;
	m_chpDataFieldEnd = m_chpBuffer + m_iBufferSize;

	m_chpReadPos = m_chpWritePos = m_chpDataFieldStart;

	m_iDataSize = 0;
}


//////////////////////////////////////////////////////////////////////////
// 버퍼 Pos 이동. (음수이동은 안됨)
// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
//
// Parameters: (int) 이동 사이즈.
// Return: (int)이동된 사이즈.
//////////////////////////////////////////////////////////////////////////
int CAyaPacket::MoveWritePos(int iSize)
{
	if ( 0 > iSize ) return 0;

	/*-----------------------------------------------------------------
	 이동할 자리가 부족하다면.
	-------------------------------------------------------------------*/
	if ( m_chpWritePos + iSize > m_chpDataFieldEnd )
		return 0;

	m_chpWritePos += iSize;
	m_iDataSize += iSize;

	return iSize;
}

//////////////////////////////////////////////////////////////////////////
// 버퍼 Pos 이동. (음수이동은 안됨)
// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
//
// Parameters: (int) 이동 사이즈.
// Return: (int)이동된 사이즈.
//////////////////////////////////////////////////////////////////////////
int CAyaPacket::MoveReadPos(int iSize)
{
	if ( 0 > iSize ) return 0;

	/*-----------------------------------------------------------------
	 이동할만큼 데이타가 없다면.
	-------------------------------------------------------------------*/
	if ( iSize > m_iDataSize )
		return 0;

	m_chpReadPos += iSize;
	m_iDataSize -= iSize;

	return iSize;
}








//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
// 연산자 오퍼레이터.
//////////////////////////////////////////////////////////////////////////
CAyaPacket &CAyaPacket::operator = (CAyaPacket &cSrcAyaPacket)
{
	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;

	/*-----------------------------------------------------------------
	원본 패킷 클래스에서 복사해온다.
	-------------------------------------------------------------------*/
	PutData(cSrcAyaPacket.m_chpReadPos, cSrcAyaPacket.m_iDataSize);

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 넣기.	각 변수 타입마다 모두 만듬.
//
//
//////////////////////////////////////////////////////////////////////////
CAyaPacket &CAyaPacket::operator << (BYTE byValue)
{
	PutData(reinterpret_cast<char *>(&byValue), sizeof(BYTE));
	return *this;
}

CAyaPacket &CAyaPacket::operator << (char chValue)
{
	PutData(&chValue, sizeof(char));
	return *this;
}

CAyaPacket &CAyaPacket::operator << (short shValue)
{
	PutData(reinterpret_cast<char *>(&shValue), sizeof(short));
	return *this;
}

CAyaPacket &CAyaPacket::operator << (WORD wValue)
{
	PutData(reinterpret_cast<char *>(&wValue), sizeof(WORD));
	return *this;
}

CAyaPacket &CAyaPacket::operator << (int iValue)
{
	PutData(reinterpret_cast<char *>(&iValue), sizeof(int));

	return *this;
}

CAyaPacket &CAyaPacket::operator << (DWORD dwValue)
{
	PutData(reinterpret_cast<char *>(&dwValue), sizeof(DWORD));
	return *this;
}

CAyaPacket &CAyaPacket::operator << (float fValue)
{
	PutData(reinterpret_cast<char *>(&fValue), sizeof(float));
	return *this;
}

CAyaPacket &CAyaPacket::operator << (__int64 iValue)
{
	PutData(reinterpret_cast<char *>(&iValue), sizeof(__int64));
	return *this;
}

CAyaPacket &CAyaPacket::operator << (double dValue)
{
	PutData(reinterpret_cast<char *>(&dValue), sizeof(double));
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 빼기.	각 변수 타입마다 모두 만듬.
//////////////////////////////////////////////////////////////////////////
CAyaPacket &CAyaPacket::operator >> (BYTE &byValue)
{
	GetData(reinterpret_cast<char *>(&byValue), sizeof(BYTE));
	return *this;
}

CAyaPacket &CAyaPacket::operator >> (char &chValue)
{
	GetData(&chValue, sizeof(char));
	return *this;
}

CAyaPacket &CAyaPacket::operator >> (short &shValue)
{
	GetData(reinterpret_cast<char *>(&shValue), sizeof(short));
	return *this;
}

CAyaPacket &CAyaPacket::operator >> (WORD &wValue)
{
	GetData(reinterpret_cast<char *>(&wValue), sizeof(WORD));
	return *this;
}

CAyaPacket &CAyaPacket::operator >> (int &iValue)
{
	GetData(reinterpret_cast<char *>(&iValue), sizeof(int));
	return *this;
}

CAyaPacket &CAyaPacket::operator >> (DWORD &dwValue)
{
	GetData(reinterpret_cast<char *>(&dwValue), sizeof(DWORD));
	return *this;
}

CAyaPacket &CAyaPacket::operator >> (float &fValue)
{
	GetData(reinterpret_cast<char *>(&fValue), sizeof(float));
	return *this;
}

CAyaPacket &CAyaPacket::operator >> (__int64 &iValue)
{
	GetData(reinterpret_cast<char *>(&iValue), sizeof(__int64));
	return *this;
}

CAyaPacket &CAyaPacket::operator >> (double &dValue)
{
	GetData(reinterpret_cast<char *>(&dValue), sizeof(double));
	return *this;
}







//////////////////////////////////////////////////////////////////////////
// 데이타 얻기.
//
// Parameters: (char *)Dest 포인터. (int)Size.
// Return: (int)복사한 사이즈.
//////////////////////////////////////////////////////////////////////////
int CAyaPacket::GetData(char *chpDest, int iSize)
{
	/*-----------------------------------------------------------------
	얻고자 하는 만큼의 데이타가 없다면.
	-------------------------------------------------------------------*/
	if ( iSize > m_iDataSize )
		return 0;

	memcpy(chpDest, m_chpReadPos, iSize);
	m_chpReadPos += iSize;

	m_iDataSize -= iSize;

	return iSize;

}


//////////////////////////////////////////////////////////////////////////
// 데이타 삽입.
//
// Parameters: (char *)Src 포인터. (int)SrcSize.
// Return: (int)복사한 사이즈.
//////////////////////////////////////////////////////////////////////////
int CAyaPacket::PutData(char *chpSrc, int iSrcSize)
{
	/*-----------------------------------------------------------------
	넣을 자리가 없다면.
	-------------------------------------------------------------------*/
	if ( m_chpWritePos + iSrcSize > m_chpDataFieldEnd )
		return 0;

	memcpy(m_chpWritePos, chpSrc, iSrcSize);
	m_chpWritePos += iSrcSize;

	m_iDataSize += iSrcSize;

	return iSrcSize;
}
