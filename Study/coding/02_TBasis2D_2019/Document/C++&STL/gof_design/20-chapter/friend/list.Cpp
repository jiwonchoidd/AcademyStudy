#include <iostream>
#include "list.h"
#include "iter.h"
using namespace std;

int
AbstractList::Count() { return totalCnt_; }

// ---------------------------------------------------

Iterator *
LinkedList::CreateIterator() {
  return new ListIterator(this);
}

void 
LinkedList::AddNext(Item *pNewItem, Item *pItem) {
  if (pFirst_ == 0) 
    pFirst_ = new LinkedItem(pNewItem);
  else if (pItem == 0 || pItem == pFirst_->pData_) {
    LinkedItem* pTmp = pFirst_->pNext_;
    pFirst_->pNext_ = new LinkedItem(pNewItem, pTmp);
  }
  else {
    LinkedItem* pPrev = 0;
    LinkedItem* pTmp = pFirst_;
    while (pTmp != 0 && pTmp->pData_ != pItem) {
      pPrev = pTmp;
      pTmp = pTmp->pNext_;
    }

    if (pTmp != 0) {
      LinkedItem* pTmp2 = pTmp->pNext_;
      pTmp->pNext_ = new LinkedItem(pNewItem, pTmp2);
    }
    else
      pPrev->pNext_ = new LinkedItem(pNewItem, 0);
  }
  totalCnt_++;
}

void 
LinkedList::AddChild(Item *pNewItem, Item *pItem) { 
  AddNext(pNewItem, pItem); 
}

void 
LinkedList::Remove(Item *pItem) {
  if (pItem == 0) return ;

  LinkedItem *pPrev = 0;
  LinkedItem *pTmp = pFirst_;
  while (pTmp != 0 && pTmp->pData_ != pItem) {
    pPrev = pTmp;
    pTmp = pTmp->pNext_;
  }

  if (pTmp != 0) {
    if (pTmp == pFirst_) {
      delete pTmp;
      pFirst_ = 0;
    }
    else {
      pPrev->pNext_ = pTmp->pNext_;
      delete pTmp;
    }
    totalCnt_--;
  }
}

Item* 
LinkedList::GetItem(int pos) {
  int cnt = 0;
  LinkedItem *pTmp = pFirst_;
  while (pTmp != 0 && cnt != pos) {
    pTmp = pTmp->pNext_;
    cnt++;
  }

  if (pTmp != 0) return pTmp->pData_;
  else           return 0;
}

// ---------------------------------------------------

Iterator *
TreeList::CreateIterator() {
  return new TreeIterator(this);
}

void 
TreeList::AddNext(Item *pNewItem, Item *pItem) {
  if (pRoot_ == 0)  // -- 처음 등록하는 경우
    pRoot_ = new TreeItem(pNewItem);
  else if (pItem == 0 || pItem == pRoot_->pData_) {
    // -- Root 항목의 첫번째 자식으로 삽입
    TreeItem *pTmp = pRoot_->pFirstChild_;
    pRoot_->pFirstChild_ = new TreeItem(pNewItem, pRoot_, 0, pTmp);
  }
  else {
    // -- pData_ 값이 pItem과 같은 항목의 형제 노드로 삽입
    TreeItem *pPrev = 0;
    TreeItem *pTmp = GetNext(pRoot_);
    while (pTmp != 0 && pTmp->pData_ != pItem) {
      pPrev = pTmp;
      pTmp = GetNext(pTmp);
    }

    if (pTmp != 0) {
      TreeItem *pTmp2 = pTmp->pSibling_;
      pTmp->pSibling_ = new TreeItem(pNewItem, pTmp->pParent_, 0, pTmp2);
    }
    else // -- 같은 노드가 없으면 맨 마지막 노드의 형제로 삽입
      pPrev->pSibling_ = new TreeItem(pNewItem, pPrev->pParent_, 0, 0);
  }
  totalCnt_++;
}

void 
TreeList::AddChild(Item *pNewItem, Item *pItem) {
  if (pRoot_ == 0)
    pRoot_ = new TreeItem(pNewItem);
  else if (pItem == 0 || pItem == pRoot_->pData_) {
    // -- Root 항목의 첫번째 자식으로 삽입
    TreeItem *pTmp = pRoot_->pFirstChild_;
    pRoot_->pFirstChild_ = new TreeItem(pNewItem, pRoot_, 0, pTmp);
  }
  else {
    // -- pData_ 값이 pItem과 같은 항목의 자식 노드로 삽입
    TreeItem *pPrev = 0;
    TreeItem *pTmp = GetNext(pRoot_);
    while (pTmp != 0 && pTmp->pData_ != pItem) {
      pPrev = pTmp;
      pTmp = GetNext(pTmp);
    }

    if (pTmp != 0) {
      TreeItem *pTmp2 = pTmp->pFirstChild_;
      pTmp->pFirstChild_ = new TreeItem(pNewItem, pTmp, 0, pTmp2);
    }
    else // -- 같은 노드가 없으면 맨 마지막 노드의 자식으로 삽입
      pPrev->pFirstChild_ = new TreeItem(pNewItem, pPrev, 0, 0);
  }
  totalCnt_++;
}

void 
TreeList::Remove(Item *pItem) {
  if (pItem == 0) return;

  TreeItem *pPrev = 0;
  TreeItem *pTmp = pRoot_;
  while (pTmp != 0 && pTmp->pData_ != pItem) {
    pPrev = pTmp;
    pTmp = GetNext(pTmp);
  }

  if (pTmp != 0) {
    if (pTmp->pFirstChild_ != 0) 
      cout << "Can't Remove the requested Node" << endl;
    else {
      if (pTmp == pRoot_) {
        delete pTmp;
        pRoot_ = 0;
      }
      else if (pTmp->pParent_->pFirstChild_ == pTmp) {
        // -- 지우려는 항목이 첫번째 자식 노드일 경우
        pTmp->pParent_->pFirstChild_ = GetNext(pTmp);
        delete pTmp;
      }
      else {
        pPrev->pSibling_ = pTmp->pSibling_;
        delete pTmp;
      }
      totalCnt_--;
    }
  }
}

Item* 
TreeList::GetItem(int pos) {
  int cnt = 0;
  TreeItem* pTmp = pRoot_;
  while (pTmp != 0 && cnt != pos) {
    pTmp = GetNext(pTmp);
    cnt++;
  }

  if (pTmp != 0) return pTmp->pData_;
  else           return 0;
}

TreeList::TreeItem* 
TreeList::GetNext(const TreeItem* pStart) { // -- Bread First 방식
  if (pStart == 0) return 0;
  else if (pStart->pSibling_ != 0)  // -- 형제 노드가 존재시
    return pStart->pSibling_;
  else if (pStart->pParent_ == 0) // -- Root 노드의 경우
    return pStart->pFirstChild_;
  else   // -- 현재 레벨에서 마지막 형제 노드의 경우
    return pStart->pParent_->pFirstChild_->pFirstChild_;
}
