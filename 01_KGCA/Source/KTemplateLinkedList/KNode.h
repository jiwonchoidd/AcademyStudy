#pragma once
#include <iostream>
template <class Type>
class KNode
{
public:
	Type*	m_pValue;
	KNode*	m_pNext;

public:
	void Delete()
	{
		delete m_pValue;
	}
public:
	KNode();
	~KNode();
};

template<class Type>
KNode<Type>::KNode()
{
	m_pValue = nullptr;
	m_pNext = nullptr;
}

template<class Type>
KNode<Type>::~KNode()
{
	m_pValue = nullptr;
	delete m_pValue;
}
