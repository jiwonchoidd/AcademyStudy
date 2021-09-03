#define _SCL_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector> // �迭 ���Ҵ�
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
	// �⺻ ������ �����Ƿ� ���.
	data1.sort(less<>()); //operator <
	data1.reverse();
	// ������ ������ �������� ���
	data1.sort(); // operator <
	data1.reverse();
	// �����Լ��� ���Ǵ��Ͽ� �����Ѵ�.
	data1.sort(cmp);
	data1.reverse();	

	//operator() (TStudent& a, TStudent& b)  ȣ��
	data1.sort(TStudent());


	TCheck<TStudent> check;
	check.m_iValue = 55;
	list<TStudent>::iterator pos;
	pos = find_if(data1.begin(), data1.end(), check );
	// ������������� ����Ͽ� �˻��Ѵ�.
	pos = find_if(data1.begin(), data1.end(), TStudent() );

	for_each(data1.begin(), data1.end(), print);

	
	
	list<TStudent*> data2;
	for (int i = 0; i < 10; i++)
	{
		TStudent* a = new TStudent;
		data2.push_back(a);
	}
	////////// �������� �ּҰ����� ���ĵȴ�. ////////	
	data2.sort();
	// �����Լ��� ���Ǵ��Ͽ� �����Ѵ�.
	data2.sort(cmpPtr);

	TCheckPtr<TStudent*> checkPtr;
	checkPtr.m_iValue = 55;
	list<TStudent*>::iterator posPtr;
	posPtr = find_if(data2.begin(), data2.end(), checkPtr);
	if (posPtr == data2.end())
	{
		cout << "none";
	}

	// ������������� ����Ͽ� �˻��Ѵ�.
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