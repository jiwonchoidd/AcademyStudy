#include "TStudent.h"


void	TStudent::SetIndex(int iIndex)
{
	m_iIndex = iIndex;
};
void	TStudent::SetName( wstring name )
{
	m_szName = name;
};
wstring	TStudent::GetName()
{
	if( this == NULL )
	{
		throw 0;
	}
	/*try{
		if( this == NULL )
		{
			ExceptB();
		}
	}
	catch(ExceptB& ex)
	{
		cout << "Catch ExceptA"<<endl;
		ex.What();
	}*/
	return m_szName;
};
void	TStudent::SetSubject( int m_iKor,
					int m_iEng, int m_iMat )
{
	m_Subject.Set( m_iKor, m_iEng, m_iMat);
	m_iTotal =  m_iKor + m_iEng + m_iMat;
	m_fAverage = m_iTotal / 3.0f;
};
TSubject TStudent::GetSubject()
{
	
	return m_Subject;
};
int		TStudent::GetIndex() 
{
		return m_iIndex;
}
int		TStudent::GetTotal() 
{
		return m_iTotal;
}
float	TStudent::GetAverage()
{
	return m_fAverage;
}
TStudent::TStudent(void)
{
	m_fAverage = 0.0f;
	m_iTotal = 0.0f;
	m_iIndex = -1;
	//m_pNext  = NULL;
	//m_pPrev  = NULL;
}


TStudent::~TStudent(void)
{
	//cout << "TStudent" << endl;
}
