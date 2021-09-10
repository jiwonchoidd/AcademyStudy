#include "HashTable.h"

HashTable::HashTable( int tableLength )
{
    if (tableLength <= 0) tableLength = 13;
    m_pArray = new LinkedList[ tableLength ];
    m_pLength = tableLength;
}


int HashTable::hash( string itemKey )
{
    int value = 0;
    for (int i = 0; i < itemKey.length(); i++)
        value += itemKey[i];
    return (itemKey.length() * value) % m_pLength;
}

void HashTable::insertItem( Item * newItem )
{
    int index = hash( newItem -> key );
    m_pArray[ index ].insertItem( newItem );
}

bool HashTable::removeItem( string itemKey )
{
    int index = hash( itemKey );
    return m_pArray[ index ].removeItem( itemKey );
}

Item * HashTable::getItemByKey( string itemKey )
{
    int index = hash( itemKey );
    return m_pArray[index].getItem( itemKey );
}

void HashTable::printTable()
{
    cout << "\nHash Table:\n";
    for (int i = 0; i < m_pLength; i++)
    {
        cout << "Bucket " << i+1 << ": ";
        m_pArray[i].printList();
    }
}

void HashTable::printHistogram()
{
    cout << "\n\nHash Table Contains ";
    cout << getNumberOfItems() << " Items total\n";
    for (int i = 0; i < m_pLength; i++)
    {
        cout << i + 1 << ":\t";
        for (int j = 0; j < m_pArray[i].getLength(); j++)
            cout << " X";
        cout << "\n";
    }
}

int HashTable::getLength()
{
    return m_pLength;
}

int HashTable::getNumberOfItems()
{
    int itemCount = 0;
    for (int i = 0; i < m_pLength; i++)
        itemCount += m_pArray[i].getLength();
    return itemCount;
}

HashTable::~HashTable()
{
    delete [] m_pArray;
}


