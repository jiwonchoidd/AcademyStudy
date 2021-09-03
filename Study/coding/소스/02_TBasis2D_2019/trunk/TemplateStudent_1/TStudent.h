#pragma once
#include "TSubject.h"
class ExceptB
{
public:
	void What()
	{
		cout << "TStudent::GetName() this==NULL" << endl;
	}
};

class TStudent
{
private:
	int			m_iIndex;
	wstring		m_szName;
	TSubject	m_Subject;
	int			m_iTotal;
	float		m_fAverage;
public:
	void		SetIndex( int iIndex );
	int			GetIndex();
	void		SetName( wstring name );
	wstring		GetName();
	void		SetSubject( int m_iKor,	int m_iEng, int m_iMat );
	TSubject	GetSubject();
	int			GetTotal();
	float		GetAverage();
public:
	TStudent(void);
	~TStudent(void);
};

