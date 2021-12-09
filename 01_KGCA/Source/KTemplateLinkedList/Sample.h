#pragma once
#include "KTemplateLinkedList.h"
class Person
{
public:
	int		m_Value;
	Person()
	{
		m_Value = 0;
	}
};
class Sample
{
public:
	KTemplateLinkedList<Person> m_LL;
};
