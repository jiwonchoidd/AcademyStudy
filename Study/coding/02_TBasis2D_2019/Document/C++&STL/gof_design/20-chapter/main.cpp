#include <iostream>
#include "item.h"
#include "list.h"
#include "iter.h"
using namespace std;

void
PrintData(Iterator* pIter)
{
  for (pIter->First(); ! pIter->IsDone(); pIter->Next()) 
    cout << pIter->GetCurItem()->data_ << endl;
}

void
main()
{
  Item item1("A"), item2("B"), item3("C"), item4("D");

  LinkedList list;
  list.AddNext(&item1);
  list.AddNext(&item2, &item1);
  list.AddNext(&item3, &item2);
  list.AddNext(&item4, &item3);

  ListIterator *pListIter = 
	  (ListIterator*) list.CreateIterator();
  PrintData(pListIter);

  cout << "--------------------------------" << endl;

  TreeList tree;
  tree.AddNext(&item1);
  tree.AddNext(&item2, &item1);
  tree.AddChild(&item3, &item2);
  tree.AddChild(&item4, &item2);

  TreeIterator *pTreeIter = 
	  (TreeIterator *) tree.CreateIterator();
  PrintData(pListIter);
}
