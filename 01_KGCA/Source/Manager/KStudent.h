#pragma once
#include "KTemplateLinkedList.h"
class KStudent
{
public:
	std::string	m_Name;
	int		m_KorScore;
	int		m_MathScore;
	int		m_EngScore;
	int		m_Hap;
	int		m_Avg;
	KStudent()
	{
		m_Name = "NoName";
		m_KorScore = 0;
		m_MathScore = 0;
		m_EngScore = 0;
		m_Hap = 0;
		m_Avg = 0;
	}
	KStudent(std::string name, int korea, int math, int eng)
	{
		m_Name = name;
		m_KorScore = korea;
		m_MathScore = math;
		m_EngScore = eng;
		m_Hap = korea + math + eng;
		m_Avg = m_Hap / 3;
	}
};

