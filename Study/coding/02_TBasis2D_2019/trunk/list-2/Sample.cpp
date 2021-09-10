#define _SCL_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector> // 배열 재할당
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
using namespace std;

class TStudent
{
public:
	int m_iType;
	int m_iIndex;
	std::string name;
	static int g_FindIndex;
	bool operator() (TStudent& val) const
	{
		return (val.m_iIndex > m_iIndex);
	}
	bool operator() (const TStudent* val)  const
	{
		return (val->m_iIndex == g_FindIndex);
	}	
	bool operator() (TStudent& a, TStudent& b) 
	{
		return (a.m_iIndex > b.m_iIndex);
	}
	bool operator < (TStudent& val) const
	{
		return (val.m_iIndex > m_iIndex);
	}
	void print() const
	{
		std::cout << m_iIndex << " ";
	}
	void printMsg(int  idata) const
	{
		std::cout << idata << ":" << m_iIndex << " ";
	}
	TStudent()
	{
		m_iIndex = rand() % 50;
	}	
};
int TStudent::g_FindIndex = 0;

template<typename T>
class TCheck
{
public:
	bool operator() (T val) const
	{
		return (val.m_iIndex == m_iValue);
	}
	int m_iValue;
};
template<typename T>
class TCheckPtr
{
public:
	bool operator() (T val) const
	{
		return (val->m_iIndex == m_iValue);
	}
	int m_iValue;
};
void printPoint(const TStudent* pData)
{
	cout << pData->m_iIndex << " ";
}
void print(TStudent& data)
{
	cout << data.m_iIndex << " ";
}
bool cmp(TStudent& a, TStudent& b)
{
	return (a.m_iIndex > b.m_iIndex);
}
bool cmpPtr(TStudent* a, TStudent* b)
{
	return (a->m_iIndex > b->m_iIndex);
}
void main()
{
	list<TStudent> data1;
	for (int i = 0; i < 10; i++)
	{
		TStudent a;
		data1.push_back(a);
	}
	// 기본 연산자 재정의로 사용.
	data1.sort(less<>()); //operator <
	data1.reverse();
	// 연산자 재정의 구현으로 사용
	data1.sort(); // operator <
	data1.reverse();
	// 전역함수로 비교판단하여 정렬한다.
	data1.sort(cmp);
	data1.reverse();	

	//operator() (TStudent& a, TStudent& b)  호출
	data1.sort(TStudent());


	TCheck<TStudent> check;
	check.m_iValue = 55;
	list<TStudent>::iterator pos;
	pos = find_if(data1.begin(), data1.end(), check );
	// 정적멤버변수를 사용하여 검색한다.
	pos = find_if(data1.begin(), data1.end(), TStudent() );

	for_each(data1.begin(), data1.end(), print);

	
	
	list<TStudent*> data2;
	for (int i = 0; i < 10; i++)
	{
		TStudent* a = new TStudent;
		data2.push_back(a);
	}
	////////// 포인터의 주소값으로 정렬된다. ////////	
	data2.sort();
	// 전역함수로 비교판단하여 정렬한다.
	data2.sort(cmpPtr);

	TCheckPtr<TStudent*> checkPtr;
	checkPtr.m_iValue = 55;
	list<TStudent*>::iterator posPtr;
	posPtr = find_if(data2.begin(), data2.end(), checkPtr);
	if (posPtr == data2.end())
	{
		cout << "none";
	}

	// 정적멤버변수를 사용하여 검색한다.
	TStudent::g_FindIndex = 41;
	posPtr = find_if(data2.begin(), data2.end(), TStudent());
	if (posPtr != data2.end())
	{
		cout << (*posPtr)->m_iIndex;
	}
	for_each(data2.begin(), data2.end(), printPoint);


	while( data2.size() > 0)
	{
		TStudent* a = *data2.begin();
		delete a;
		data2.erase(data2.begin());
	}

	data1.clear();
	data2.clear();
}