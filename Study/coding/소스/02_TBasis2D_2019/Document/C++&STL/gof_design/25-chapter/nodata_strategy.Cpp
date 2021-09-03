#include <iostream>
#include <stdlib.h>
using namespace std;

#define DEFAULT_MAX_ITEMS     100

class SortAlgorithm {
  public :
    static SortAlgorithm* CreateInstance() { return 0; }
    virtual void Sort(int numOfItems, int *pArray) = 0;
  protected :
    SortAlgorithm() {}
};

class BubbleSort : public SortAlgorithm {
  public :
    static BubbleSort* CreateInstance() {
      if (pInstance_ == 0) pInstance_ = new BubbleSort;
      return pInstance_;
    }

    void Sort(int numOfItems, int *pArray) {
      cout << "Bubble Sort Algorithm" << endl;
    }
  protected :
    BubbleSort() {}
  private :
    static BubbleSort *pInstance_;
};
BubbleSort* BubbleSort::pInstance_ = 0;

class QuickSort : public SortAlgorithm {
  public :
    static QuickSort* CreateInstance() {
      if (pInstance_ == 0) pInstance_ = new QuickSort;
      return pInstance_;
    }

    void Sort(int numOfItems, int *pArray) {
      cout << "Quick Sort Algorithm" << endl;
    }
  protected :
    QuickSort() {}
  private :
    static QuickSort *pInstance_;
};
QuickSort* QuickSort::pInstance_ = 0;

class IntegerArray {
  public :
    IntegerArray(int numOfItems=DEFAULT_MAX_ITEMS) {
      pData_ = new int [numOfItems];
      numOfItems_ = numOfItems;
    }
    void CreateData() {
      for (int i=0; i < numOfItems_; i++)
        pData_[i] = rand() % 32768;
    }

    void PrintOut() {
      for (int i=0; i < numOfItems_; i++)
        cout << pData_[i] << " ";
      cout << endl;
    }

    void Sort(SortAlgorithm *pSortAlgorithm) { 
      pSortAlgorithm->Sort(numOfItems_, pData_); 
    }
  private :
    int numOfItems_;
    int *pData_;
};

void
main()
{
  IntegerArray a(10);
  a.CreateData();

  BubbleSort *pBubbleSort = BubbleSort::CreateInstance();
  a.Sort(pBubbleSort); 

  QuickSort *pQuickSort = QuickSort::CreateInstance();
  a.Sort(pQuickSort);
}
