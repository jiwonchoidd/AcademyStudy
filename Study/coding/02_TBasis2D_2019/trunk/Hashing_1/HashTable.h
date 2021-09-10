#ifndef HashTable_h
#define HashTable_h
#include "LinkedList.h"

class HashTable
{
private:
    LinkedList * m_pArray;  
    int			 m_pLength;
    int hash( string itemKey );
    
public:       
    HashTable( int tableLength = 13 );
    void insertItem( Item * newItem );
    bool removeItem( string itemKey );
    Item * getItemByKey( string itemKey );
    void printTable();
    void printHistogram();
    int getLength();
    int getNumberOfItems();
    ~HashTable();
};
#endif

