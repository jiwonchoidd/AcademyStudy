#include "TControlMgr.h"
template <typename T> void SwapPoint(T* a, T* b)
{
	a->m_pNext = b;
	b->m_pPrev = a;
}
TNode<TStudent>* TControlMgr::Swap(TNode<TStudent>* a, TNode<TStudent>* b) // 1, 2
{
	// NULL
	TNode<TStudent>* aprev = a->m_pPrev; 
	TNode<TStudent>* anext = a->m_pNext; 
	TNode<TStudent>* bprev = b->m_pPrev; 
	TNode<TStudent>* bnext = b->m_pNext; 

	//Head <-> b 
	if( aprev != NULL ) 
	{
		SwapPoint<TNode<TStudent>>(aprev, b);
	}
	else
	{
		m_List.m_pHead = b;
		b->m_pPrev = NULL;
	}
	// b <-> c    anext(c)
	if( anext != b )	
	{
		SwapPoint<TNode<TStudent>>(b, anext);
		// 초기 a -> c -> b 경우
		if( anext->m_pNext == b )
		{
			SwapPoint<TNode<TStudent>>(anext, a);
		}
		else
		{
			SwapPoint<TNode<TStudent>>(bprev, a);
		}
	}
	else  // b <-> a
	{
		b->m_pNext = a;      // b -> a
		a->m_pPrev = b;      // b <- a
		a->m_pNext = bnext;  // a -> d
		if( bnext != NULL )	bnext->m_pPrev = a;  // a <- d		
	}
	// a <-> d  anext(d)  // 초기 a -> c -> b -> d 경우
	if( bnext != NULL )	
	{
		SwapPoint<TNode<TStudent>>(a, bnext);
	}
	else
	{
		a->m_pNext = NULL;  // a -> d
	}

	if( anext == b )	return a;
	return anext;
}

void TControlMgr::Sort(bool bUp)
{
	//m_List.Sort( bUp );
	//  정렬
	TNode<TStudent>* pNodeSrc  = m_List.GetHead();
	// 가장 작은 것을 앞으로  이동시킨다.
	while(pNodeSrc && pNodeSrc->m_pNext )
	{
		TNode<TStudent>* pNodeSwap = pNodeSrc;
		TStudent*  pStd= pNodeSwap->m_pData;

		for(TNode<TStudent>*  pNodeDesk=pNodeSrc->m_pNext;
					pNodeDesk != NULL;
					pNodeDesk = pNodeDesk->m_pNext )
		{
			TStudent*  pDesk= pNodeDesk->m_pData;
			if( pNodeSwap->m_pData->GetTotal() > pDesk->GetTotal() )
			{
				pNodeSwap = pNodeDesk;			
			}
		}
		if(pNodeSrc != pNodeSwap ) // pNodeSrc보다 크거나 작은 것이 있다.
		{
			pNodeSrc = Swap( pNodeSrc, pNodeSwap);			
		}
		else		
		{
			pNodeSrc = pNodeSrc->m_pNext;// pNodeSrc보다 크거나 작은 것이 없다.
		}
	}
	m_List.m_pTail = pNodeSrc;
}
TStudent* TControlMgr::Find(int iCnt)
{	
	TStudent* pData = m_List.GetData(iCnt);
	return pData;	
}
void TControlMgr::ShowData()
{	
	for(TNode<TStudent>*  pNode=m_List.GetHead();
					pNode != NULL;
					pNode = pNode->m_pNext )
	{
		TStudent*  pData= pNode->m_pData;
		if( pData )
		{
			wcout << pData->GetIndex() 
				<< L"   " 
				<< pData->GetName() 
				<< L"   " 
				<< pData->GetTotal() << endl;
		}
	}
}
void    TControlMgr::ShowData(TStudent* pStd)
{
	if (pStd == NULL) return;
	wcout << " ========= " << pStd->GetIndex() << " "
		<< pStd->GetName().c_str() << " "
		<< pStd->GetSubject().GetKor() << " "
		<< pStd->GetSubject().GetEng() << " "
		<< pStd->GetSubject().GetMat() << " "
		<< pStd->GetTotal() << " "
		<< endl;
}
bool TControlMgr::InputData(int iCnt)
{
	for( int iSt=0; iSt < iCnt; iSt++)
	{		
		TStudent* pStudent = new TStudent;
		wstring name;
		for( int iname =0; iname < 3; iname++ )
		{
			char iName = 65+rand()%26;
			name += iName;
		}
		pStudent->SetIndex(iSt);
		pStudent->SetName(name);
		pStudent->SetSubject( rand() %100,rand() %100,rand() %100 );
				
		m_Hash.Insert(pStudent);
		m_List.AddLink( pStudent );
	}
	return true;
}
void TControlMgr::Release()
{
	m_List.Delete();
}
TControlMgr::TControlMgr(void)
{
}


TControlMgr::~TControlMgr(void)
{
}
