#include "TValue.h"


namespace A_TYPE
{
	TValue::TValue(TCHAR* pData)
	{
		int iLength = _tcslen(pData) + 1;
		int iSize = sizeof(TCHAR) * iLength;
		this->m_szValue = new TCHAR[iSize];
		_tcscpy_s(m_szValue, iSize, pData);
		m_pNext = NULL;
	}
	TValue::~TValue()
	{
		delete m_szValue;
	}
}
namespace B_TYPE
{
	TValue::TValue(TCHAR* pData)
	{
		int iLength = _tcslen(pData) + 1;
		int iSize = sizeof(TCHAR) * iLength;
		this->m_szValue = new TCHAR[iSize];
		_tcscpy_s(m_szValue, iSize, pData);
		m_pNext = NULL;
	}
	TValue::~TValue()
	{
		delete m_szValue;
	}
}
