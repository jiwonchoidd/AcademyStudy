

/*---------------------------------------------------------------

	Aya Library - Packet.

	��Ʈ��ũ ��Ŷ�� Ŭ����.
	�����ϰ� ��Ŷ�� ������� ����Ÿ�� In, Out �Ѵ�.

	- ����.

	CAyaPacket clPacket;

	�ֱ�.
	clPacket << 40030;	or	clPacket << iValue;	(int �ֱ�)
	clPacket << 3;		or	clPacket << byValue;	(BYTE �ֱ�)
	clPacket << 1.4;		or	clPacket << fValue;	(float �ֱ�)

	����.
	clPacket >> iValue;	(int ����)
	clPacket >> byValue;	(BYTE ����)
	clPacket >> fValue;	(float ����)

	!.	���ԵǴ� ����Ÿ FIFO ������ �����ȴ�.
		ť�� �ƴϹǷ�, �ֱ�(<<).����(>>) �� ȥ���ؼ� ����ϸ� �ȵȴ�.
		
----------------------------------------------------------------*/
#include <windows.h>
#include <assert.h>
#include "AyaPacket.h"


//////////////////////////////////////////////////////////////////////////
// ������, �ı���.
//
// Parameters:
//				---------------------------------------------------------
//				(int) ���� ������.
//				---------------------------------------------------------
//				(const CAyaPacket &) ��Ŷ�� ���縦 ���� ��Ŷ ���۷���.
// Return:
//////////////////////////////////////////////////////////////////////////
CAyaPacket::CAyaPacket() :	m_chpBuffer(NULL),
							m_chpDataFieldStart(NULL),
							m_chpDataFieldEnd(NULL),
							m_chpReadPos(NULL),
							m_chpWritePos(NULL)
{
	/*-----------------------------------------------------------------
	���� ����� �Է����� �ʴ´ٸ�, �⺻ġ�� ����.
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
	���� ��Ŷ Ŭ�������� �����ؿ´�.
	-------------------------------------------------------------------*/
	PutData(cSrcAyaPacket.m_chpReadPos, cSrcAyaPacket.m_iDataSize);
	
}


CAyaPacket::~CAyaPacket()
{
	Release();
}






//////////////////////////////////////////////////////////////////////////
// ��Ŷ �ʱ�ȭ.
//
// Parameters: (int)BufferSize.
// Return: ����.
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
// ��Ŷ  �ı�.
//
// Parameters: ����.
// Return: ����.
//////////////////////////////////////////////////////////////////////////
void CAyaPacket::Release(void)
{
	if ( NULL != m_chpBufferExpansion )
		delete[] m_chpBufferExpansion;
}


//////////////////////////////////////////////////////////////////////////
// ��Ŷ û��.
//
// Parameters: ����.
// Return: ����.
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
// ���� Pos �̵�. (�����̵��� �ȵ�)
// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
//
// Parameters: (int) �̵� ������.
// Return: (int)�̵��� ������.
//////////////////////////////////////////////////////////////////////////
int CAyaPacket::MoveWritePos(int iSize)
{
	if ( 0 > iSize ) return 0;

	/*-----------------------------------------------------------------
	 �̵��� �ڸ��� �����ϴٸ�.
	-------------------------------------------------------------------*/
	if ( m_chpWritePos + iSize > m_chpDataFieldEnd )
		return 0;

	m_chpWritePos += iSize;
	m_iDataSize += iSize;

	return iSize;
}

//////////////////////////////////////////////////////////////////////////
// ���� Pos �̵�. (�����̵��� �ȵ�)
// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
//
// Parameters: (int) �̵� ������.
// Return: (int)�̵��� ������.
//////////////////////////////////////////////////////////////////////////
int CAyaPacket::MoveReadPos(int iSize)
{
	if ( 0 > iSize ) return 0;

	/*-----------------------------------------------------------------
	 �̵��Ҹ�ŭ ����Ÿ�� ���ٸ�.
	-------------------------------------------------------------------*/
	if ( iSize > m_iDataSize )
		return 0;

	m_chpReadPos += iSize;
	m_iDataSize -= iSize;

	return iSize;
}








//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
// ������ ���۷�����.
//////////////////////////////////////////////////////////////////////////
CAyaPacket &CAyaPacket::operator = (CAyaPacket &cSrcAyaPacket)
{
	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;

	/*-----------------------------------------------------------------
	���� ��Ŷ Ŭ�������� �����ؿ´�.
	-------------------------------------------------------------------*/
	PutData(cSrcAyaPacket.m_chpReadPos, cSrcAyaPacket.m_iDataSize);

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
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
// ����.	�� ���� Ÿ�Ը��� ��� ����.
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
// ����Ÿ ���.
//
// Parameters: (char *)Dest ������. (int)Size.
// Return: (int)������ ������.
//////////////////////////////////////////////////////////////////////////
int CAyaPacket::GetData(char *chpDest, int iSize)
{
	/*-----------------------------------------------------------------
	����� �ϴ� ��ŭ�� ����Ÿ�� ���ٸ�.
	-------------------------------------------------------------------*/
	if ( iSize > m_iDataSize )
		return 0;

	memcpy(chpDest, m_chpReadPos, iSize);
	m_chpReadPos += iSize;

	m_iDataSize -= iSize;

	return iSize;

}


//////////////////////////////////////////////////////////////////////////
// ����Ÿ ����.
//
// Parameters: (char *)Src ������. (int)SrcSize.
// Return: (int)������ ������.
//////////////////////////////////////////////////////////////////////////
int CAyaPacket::PutData(char *chpSrc, int iSrcSize)
{
	/*-----------------------------------------------------------------
	���� �ڸ��� ���ٸ�.
	-------------------------------------------------------------------*/
	if ( m_chpWritePos + iSrcSize > m_chpDataFieldEnd )
		return 0;

	memcpy(m_chpWritePos, chpSrc, iSrcSize);
	m_chpWritePos += iSrcSize;

	m_iDataSize += iSrcSize;

	return iSrcSize;
}
