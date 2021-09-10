#include <iostream>
#include <stdlib.h>
using namespace std;

#define DEFAULT_MAX_ITEMS   100

#define BUBBLE_SORT_ALGORITHM    1
#define QUICK_SORT_ALGORITHM     2

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

    void Sort(int algorithmType) {
      switch(algorithmType) {
        case BUBBLE_SORT_ALGORITHM : 
          BubbleSort(); break;
        case QUICK_SORT_ALGORITHM : 
          QuickSort(); break;
        default : 
          cout << "No Match Sort Algorithm" << endl;
      }
    }
  protected :
    void BubbleSort() {
      cout << "Bubble Sort Algorithm" << endl;
    }
    void QuickSort() {
      cout << "Quick Sort Algorithm" << endl;
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
  a.Sort(QUICK_SORT_ALGORITHM);
}
