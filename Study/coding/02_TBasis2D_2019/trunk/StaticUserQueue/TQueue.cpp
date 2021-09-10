#include "TQueue.h"


void TQueue::Push(TServerObj* pObj)
{
	m_List.push(pObj);
}
TServerObj* TQueue::Pop()
{
	TServerObj* pObj = (TServerObj*)m_List.front();
	m_List.pop();
	return pObj;
}
TQueue::TQueue()
{
}


TQueue::~TQueue()
{
}
