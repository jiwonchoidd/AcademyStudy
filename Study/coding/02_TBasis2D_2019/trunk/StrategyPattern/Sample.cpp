//strategy의 의미는 '전략', '전술'이다.소프트웨어 개발에서 전략이나 전술은 알고리즘으로 구현한다.
//이 패턴을 사용하면 클라이언트와 무관하게 독립적으로 알고리즘을 변경할 수 있고(quickSort→bubbleSort), 
//클라이언트는 독립적으로 원하는 알고리즘을 사용할 수 있다.
//즉 이 패턴은 클라이언트에게 알고리즘이 사용하는 데이터나 그 구조를 숨겨주는 역할을 한다.
//strategy 패턴은 알고리즘을 사용하는 곳과, 알고리즘을 제공하는 곳을 분리시킨 구조로 
//알고리즘을 동적으로 교체할 수 있다.


#include <iostream>
#include <stdlib.h>
using namespace std;

#define DEFAULT_MAX_ITEMS     100

class SortAlgorithm 
{
public:
	static SortAlgorithm* CreateInstance() { return 0; }
	virtual void Sort(int numOfItems, int *pArray) = 0;
protected:
	SortAlgorithm() {}
};

class BubbleSort : public SortAlgorithm 
{
public:
	static BubbleSort* CreateInstance() 
	{
		if (pInstance_ == 0) pInstance_ = new BubbleSort;
		return pInstance_;
	}

	void Sort(int numOfItems, int *pArray) 
	{
		cout << "Bubble Sort Algorithm" << endl;
	}
protected:
	BubbleSort() {}
private:
	static BubbleSort *pInstance_;
};
BubbleSort* BubbleSort::pInstance_ = 0;

class QuickSort : public SortAlgorithm 
{
public:
	static QuickSort* CreateInstance() 
	{
		if (pInstance_ == 0) pInstance_ = new QuickSort;
		return pInstance_;
	}

	void Sort(int numOfItems, int *pArray) 
	{
		cout << "Quick Sort Algorithm" << endl;
	}
protected:
	QuickSort() {}
private:
	static QuickSort *pInstance_;
};
QuickSort* QuickSort::pInstance_ = 0;

template <class Strategy>
class IntegerArray 
{
public:
	IntegerArray(Strategy *pAlgorithm, int numOfItems = DEFAULT_MAX_ITEMS) 
	{
		pData_ = new int[numOfItems];
		numOfItems_ = numOfItems;
		pStrategy_ = pAlgorithm;
	}
	void CreateData() 
	{
		for (int i = 0; i < numOfItems_; i++)
			pData_[i] = rand() % 32768;
	}

	void PrintOut()
	{
		for (int i = 0; i < numOfItems_; i++)
			cout << pData_[i] << " ";
		cout << endl;
	}

	void Sort() { pStrategy_->Sort(numOfItems_, pData_); }
private:
	int numOfItems_;
	int *pData_;
	Strategy *pStrategy_;
};

void main()
{
	QuickSort *pQuickSort = QuickSort::CreateInstance();
	IntegerArray<QuickSort> a(pQuickSort, 10);
	a.CreateData();
	a.Sort();
}
