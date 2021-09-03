#include "DLinkedList.h"

DLinkedList::DLinkedList()
{
	m_pHead = new DNode;
}

DLinkedList::~DLinkedList()
{
	delete m_pHead;
	delete m_pTail;
}
