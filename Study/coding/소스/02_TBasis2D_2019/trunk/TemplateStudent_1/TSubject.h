#pragma once
#include <iostream>
#include <string>
using namespace std;

class TSubject
{
private:
	int		m_iKor;
	int		m_iEng;
	int		m_iMat;
public:
	void	Set( int m_iKor, int Eng, int m_iMat );
	int		GetKor();
	int		GetEng();
	int		GetMat();
public:
	TSubject(void);
	~TSubject(void);
};

