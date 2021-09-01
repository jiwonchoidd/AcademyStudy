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
	//static int는 클래스 내에서 초기화가 불가능하다.
	static int g_iCount;
	
public:	
	// static int g_iCount;는 밖에서 초기화가 필요하지만
	// const static int g_iCount=0;는 초기화가 가능하다.	
	// 멤버 이니셜라이져에서 초기화가 가능하다.
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

