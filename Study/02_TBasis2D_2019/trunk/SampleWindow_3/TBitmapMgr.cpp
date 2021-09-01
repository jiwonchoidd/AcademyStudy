#include "TBitmapMgr.h"

template< typename T >
void SwapPoint(T* a, T* b)
{
	a->m_pNext = b;
	b->m_pPrev = a;
}
TNode<TBitmap> * TBitmapMgr::Swap(
	TNode<TBitmap>* a, TNode<TBitmap>* b)
{
	TNode<TBitmap> * aPrev = a->m_pPrev;
	TNode<TBitmap> * aNext = a->m_pNext;
	TNode<TBitmap> * bPrev = b->m_pPrev;
	TNode<TBitmap> * bNext = b->m_pNext;

	// 1번 Haed <-> b
	if (aPrev != NULL) {
		SwapPoint(aPrev, b);
	}
	else {
		m_List.m_pHead = b;
		b->m_pPrev = NULL;
	}
	// b <-> a
	if (aNext != b)
	{
		SwapPoint(b, aNext);
		// a -> c -> b
		if (aNext->m_pNext == b)	SwapPoint(aNext, a);
		else						SwapPoint(bPrev, a);
	}
	else // b <-> a
	{
		SwapPoint(b, a);
		a->m_pNext = bNext;
		if (bNext != NULL) bNext->m_pPrev = a;
	}
	// a-> c -> b ->d
	if (bNext != NULL)  SwapPoint(a, bNext);
	else				a->m_pNext = NULL;

	if (aNext == b) return a;
	return aNext;
}
// iCnt 명에 대한 학생을 랜덤하게 구성.
TBitmap*   TBitmapMgr::Load(TCHAR* pszLoadFile)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	TCHAR SaveName[MAX_PATH] = { 0, };
	_tsplitpath_s(pszLoadFile, Drive, Dir, Name, Ext);
	_stprintf_s(SaveName, _T("%s%s"), Name, Ext);
	TBitmap* pBitmap = NULL;
	pBitmap = m_List.GetData(SaveName);
	if(pBitmap)
	{
		return pBitmap;
	}

	TBitmap* pData = new TBitmap;
	if (pData->Load(pszLoadFile))
	{
		m_Hash.Insert(pData);
		m_List.AddLink(pData);
		//return m_List.GetData(pData->m_szName);
		return m_Hash.Find(SaveName);
	}	
	return NULL;
}

TBitmap*  TBitmapMgr::Find(int iIndex)
{
	for (TNode<TBitmap>* pLink = m_List.m_pHead;
	pLink != NULL;
		pLink = pLink->m_pNext)
	{
		TBitmap* pData = pLink->m_pData;
		if (pData->m_iIndex == iIndex)
		{
			return pData;
		}
	}
	return NULL;
}
TBitmap*  TBitmapMgr::Find(TCHAR* pszName)
{
	int iIndex = this->m_Hash.Hash(pszName);
	if (iIndex >= m_Hash.m_iLength) return NULL;
	return m_Hash.m_pArray[iIndex].GetData(pszName);
}
void    TBitmapMgr::Sort(bool bUp)
{
	//TNode<TBitmap>* pNodeSrc = m_List.GetHead();
	//while (pNodeSrc && pNodeSrc->m_pNext)
	//{
	//	TNode<TBitmap>* pNodeSwap = pNodeSrc;
	//	TBitmap* pSrcStudent = pNodeSwap->m_pData;
	//	for (TNode<TBitmap>* pNodeDesk = pNodeSrc->m_pNext;
	//	pNodeDesk != NULL;
	//		pNodeDesk = pNodeDesk->m_pNext)
	//	{
	//		TBitmap* pDeskStudent = pNodeDesk->m_pData;
	//		if (pNodeSwap->m_pData->m_iTotal > pDeskStudent->m_iTotal)
	//		{
	//			pNodeSwap = pNodeDesk;
	//		}
	//	}
	//	if (pNodeSrc != pNodeSwap)
	//	{
	//		pNodeSrc = Swap(pNodeSrc, pNodeSwap);
	//	}
	//	else
	//	{
	//		pNodeSrc = pNodeSrc->m_pNext;
	//	}
	//}
}
void    TBitmapMgr::Release()
{
	m_List.Release();
}
void    TBitmapMgr::ShowData()
{
	/*for (TNode<TBitmap>* pLink = m_List.m_pHead;
	pLink != NULL;
		pLink = pLink->m_pNext)
	{
		TBitmap* pData = pLink->m_pData;
		wcout << pData->m_iIndex << " "
			<< pData->m_szName << " "
			<< pData->m_Subject.m_iKor << " "
			<< pData->m_Subject.m_iEng << " "
			<< pData->m_Subject.m_iMat << " "
			<< pData->m_iTotal << " "
			<< endl;
	}*/
}
void    TBitmapMgr::ShowData(TBitmap* pStd)
{
	/*if (pStd == NULL) return;
	wcout << " ========= " << pStd->m_iIndex << " "
		<< pStd->m_szName << " "
		<< pStd->m_Subject.m_iKor << " "
		<< pStd->m_Subject.m_iEng << " "
		<< pStd->m_Subject.m_iMat << " "
		<< pStd->m_iTotal << " "
		<< endl;*/
}

TBitmapMgr::TBitmapMgr()
{
}


TBitmapMgr::~TBitmapMgr()
{
}
