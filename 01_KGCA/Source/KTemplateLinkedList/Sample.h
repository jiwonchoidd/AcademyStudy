#pragma once
#include "KTemplateLinkedList.h"
class Person
{
public:
	char	m_Name[256] = { 0, };
	int		m_KorScore;
	int		m_MathScore;
	int		m_EngScore;
	Person()
	{
		m_KorScore	=0;
		m_MathScore	=0;
		m_EngScore	=0;
	}
};
class Sample
{
public:
	KTemplateLinkedList<Person> m_LL;
};
