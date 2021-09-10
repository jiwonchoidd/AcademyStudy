#pragma once
#include "THashTable.cpp"
#include "TStudent.h"

class TControlMgr
{
public:
	TLinkedList<TStudent> m_List;
	THashTable<TStudent>    m_Hash;
public:
	bool	InputData(int iCnt=10);
	void	ShowData();
	void    ShowData(TStudent* pStd);
	void	Release();
	void	Sort(bool bUp=true);
	TStudent* Find(int iCnt);
	TNode<TStudent>* Swap(	TNode<TStudent>*a, 
							TNode<TStudent>*b); 
public:
	TControlMgr(void);
	virtual ~TControlMgr(void);
};

