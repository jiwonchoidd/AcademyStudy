#include <iostream>
#include "iter.h"
#include "list.h"
using namespace std;

void 
Iterator::First() { curPos_ = 0; }

void 
Iterator::Next() { curPos_++; }

// ---------------------------------------------------

ListIterator::ListIterator(LinkedList *pList) {
  pList_ = pList;
}

bool 
ListIterator::IsDone() { 
  if (curPos_ >= pList_->Count()) return true;
  else return false;
}

Item* 
ListIterator::GetCurItem() { return pList_->GetItem(curPos_); }

// ---------------------------------------------------

TreeIterator::TreeIterator(TreeList* pTree) {
  pTree_ = pTree;
}

bool 
TreeIterator::IsDone() {
  if (curPos_ >= pTree_->Count()) return true;
  else return false;
}

Item* 
TreeIterator::GetCurItem() { return pTree_->GetItem(curPos_); }
