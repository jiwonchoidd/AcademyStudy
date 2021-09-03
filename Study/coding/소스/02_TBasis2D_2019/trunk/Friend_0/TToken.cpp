#include "TToken.h"
void TToken::Show(TValue* pValue)
{
	TCHAR* pszNextToken = 0;
	TCHAR* temp = _tcstok_s(pValue->m_szValue, SPA, &pszNextToken);
	if (temp != NULL)
	{
		m_pToken = new TValue(temp);
		TValue* pLink = m_pToken;
		for (m_iSize = 1;
			temp = _tcstok_s(0, SPA, &pszNextToken);
			m_iSize++)
		{
			pLink->m_pNext = new TValue(temp);
			pLink = pLink->m_pNext;
		}
	}
	
	Show();
}
void TToken::Show()
{
	TValue* pData = m_pToken;
	for (int i = 0; i < m_iSize; i++)
	{
		std::wcout << pData->m_szValue << endl;
		pData = pData->m_pNext;
	}
}
TCHAR* TToken::nth_token(int num)
{
	TValue* cursor = m_pToken;

	for (int i = 1; i<num; i++)
		if (cursor != 0)
			cursor = cursor->m_pNext;
		else
			return 0;
	return cursor->m_szValue;
}
TToken::TToken(TCHAR* pData)
{
	m_iSize = 0;
	TCHAR* temp;
	TCHAR* pszNextToken = 0;
	try {

		//int iNum = 100;
		//int iID = 1;
		//if (iID == 0) throw(iID);		
		//if (pszNextToken == 0) //throw(pszNextToken);
		//	throw Exp();//exit(1);
		//iNum /= iID;

		temp = _tcstok_s(pData, SPA, &pszNextToken);
		if (temp != NULL)
		{
			m_pToken = new TValue(temp);
			TValue* pLink = m_pToken;
			for (m_iSize = 1;temp = _tcstok_s(0, SPA, &pszNextToken);	m_iSize++)
			{
				pLink->m_pNext = new TValue(temp);
				pLink = pLink->m_pNext;
			}
		}
		
	}
	catch (int iNum)
	{
		std::wcout << _T("Error!");
	}
	catch (TCHAR* pszNextToken)
	{
		std::wcout << _T("Error!");
		throw(pszNextToken);
	}
	catch (Exp& exp)
	{
		exp.Show(_T("error"));
	}
}
TToken::TToken()
{
	m_iSize = 0;
}
TToken::~TToken()
{
	TValue* pDel;
	TValue* pLink = m_pToken;
	while (pLink != 0)
	{
		pDel = pLink;
		pLink = pLink->m_pNext;
		delete pDel;
	}
};
