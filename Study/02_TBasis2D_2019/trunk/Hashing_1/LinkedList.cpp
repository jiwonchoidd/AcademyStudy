#include "LinkedList.h"
int LinkedList::g_iCount = 0;

// ��� �̴ϼȶ��������� �ʱ�ȭ�� �����ϴ�.
LinkedList::LinkedList() : g_iMaxCount(100)
{
    m_pHead = new Item;
    m_pHead -> next = NULL;
    m_iCount = 0;
	// const static �� ����̱� ������ �� ������ �Ұ����ϴ�.
	//g_iCount = 0; // Error!
}

void LinkedList::insertItem( Item * newItem )
{
	// �ִ� ���� ����Ʈ�� �����Ѵ�.
	if (g_iMaxCount < m_iCount)
	{
		return;
	}
    if (!m_pHead -> next)
    {
        m_pHead -> next = newItem;		
        m_iCount++;
		g_iCount++;
        return;
    }
    Item * p = m_pHead;
    Item * q = m_pHead;
    while (q)
    {
        p = q;
        q = p -> next;
    }
    p -> next = newItem;
    newItem -> next = NULL;
    m_iCount++;	
}

bool LinkedList::removeItem( string itemKey )
{
    if (!m_pHead -> next) return false;
    Item * p = m_pHead;
    Item * q = m_pHead;
    while (q)
    {
        if (q -> key == itemKey)
        {
            p -> next = q -> next;
            delete q;
            m_iCount--;
			g_iCount--;
            return true;
        }
        p = q;
        q = p -> next;
    }
    return false;
}

Item * LinkedList::getItem( string itemKey )
{
    Item * p = m_pHead;
    Item * q = m_pHead;
    while (q)
    {
        p = q;
        if ((p != m_pHead) && (p -> key == itemKey))
            return p;
        q = p -> next;
    }
    return NULL;
}

void LinkedList::printList()
{
    if (m_iCount == 0)
    {
        cout << "\n{ }\n";
        return;
    }
    Item * p = m_pHead;
    Item * q = m_pHead;
    cout << "\n{ ";
    while (q)
    {
        p = q;
        if (p != m_pHead)
        {
            cout << p -> key;
            if (p -> next) cout << ", ";
            else cout << " ";
        }
        q = p -> next;
    }
    cout << "}\n";
}

int LinkedList::getLength()
{
    return m_iCount;
}

LinkedList::~LinkedList()
{
    Item * p = m_pHead;
    Item * q = m_pHead;
    while (q)
    {
        p = q;
        q = p -> next;
        if (q) delete p;
    }
}


















