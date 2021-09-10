#pragma once
#include "TValue.h"

class Exp
{
public:
	void Show(TCHAR* pData)
	{
		std::wcout << pData;
	}
public:
	Exp() {};
	~Exp() {};
};

class TToken
{
public:
	TValue*		m_pToken;
	int			m_iSize;
public:
	void		Show();
	void		Show(TValue* pValue);
	TCHAR* nth_token(int);
public:
	TToken();
	TToken(TCHAR* pData);
	~TToken();
};



