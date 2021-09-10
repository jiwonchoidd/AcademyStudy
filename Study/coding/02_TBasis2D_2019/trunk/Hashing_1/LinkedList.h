#ifndef LinkedList_h
#define LinkedList_h

#include <iostream>
#include <string>
using namespace std;

struct Item
{
    string key;
    Item * next;
};

class LinkedList
{
private:    
    Item*		m_pHead;
	int	m_iCount;
public:
	//static int�� Ŭ���� ������ �ʱ�ȭ�� �Ұ����ϴ�.
	static int g_iCount;
	
public:	
	// static int g_iCount;�� �ۿ��� �ʱ�ȭ�� �ʿ�������
	// const static int g_iCount=0;�� �ʱ�ȭ�� �����ϴ�.	
	// ��� �̴ϼȶ��������� �ʱ�ȭ�� �����ϴ�.
	const int			g_iMaxCount = 0;
public:
    LinkedList();
    void insertItem( Item * newItem );
    bool removeItem( string itemKey );
    Item * getItem( string itemKey );
    void printList();
    int getLength();
    ~LinkedList();
};

#endif

