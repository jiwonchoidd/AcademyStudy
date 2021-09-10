#ifndef ITER_H
#define ITER_H

#include "item.h"

class LinkedList;
class TreeList;

class Iterator {
  public :
    void First();
    void Next();
    virtual bool IsDone() = 0;
    virtual Item* GetCurItem() = 0;
  protected :
    int curPos_;
    Iterator() { curPos_ = 0; }
};

class ListIterator : public Iterator {
  public :
    ListIterator(LinkedList* pList);

    bool IsDone();
    Item* GetCurItem();

  private :
    LinkedList* pList_;
};

class TreeIterator : public Iterator {
  public : 
    TreeIterator(TreeList* pTree);

    bool IsDone();
    Item* GetCurItem();

  private :
    TreeList* pTree_;
};

#endif 
