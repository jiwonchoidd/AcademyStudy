#include "TSubject.h"
void	TSubject::Set( int m_iKor, int m_iEng, int m_iMat )
{
	m_iKor = m_iKor;
	m_iEng = m_iEng;
	m_iMat = m_iMat;
};
int		TSubject::GetKor()
{
	return m_iKor;
};
int		TSubject::GetEng(){
	return m_iEng;
};
int		TSubject::GetMat(){
	return m_iMat;
};

TSubject::TSubject(void)
{
	m_iKor = 0;
	m_iEng = 0;
	m_iMat = 0;
}


TSubject::~TSubject(void)
{
}
