// list_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <vector> // 연결리스트
#include <list> // 연결리스트
#include <map> // 연결리스트
#include <string> // 연결리스트

#include <set> // 연결리스트
//#include <multiset> // 연결리스트
#include <algorithm>

#include <iostream> // 연결리스트
using namespace std;
template <class T> class IsFuction
{
public:
	const bool operator() (T& val)
	{
		return (val == 'a');
	};
};
template <typename T>
struct TCheck        
{
	bool operator() (T& val) const 
	{ 
		return (val == cCompare);
	} 																										  // 비교할 요소들을 이렇게 만들어준다.
	char cCompare;
};


char vdata = 'm';
const bool Fuction(char elem)
{
	if (elem == vdata)
		return true;
	return false;
}
const bool IsFind(char elem)
{
	if (elem == vdata)
		return false;
	return true;
}

struct TData
{
	int m_iValue;
};
bool operator<(const TData& lhs, const TData& rhs)
{
	return lhs.m_iValue < rhs.m_iValue;
}

int main()
{
	// 구조체를 사용시는 비교연산자를 제공해야 정렬할 수 있다.
	std::set<TData> list;
	TData a1;
	TData a2;
	TData a3;
	list.insert(a1);
	list.insert(a2);
	list.insert(a3);
	// Set
	std::multiset<char> set1;
	set1.insert('g');// new iterate(g)
	set1.insert('a');// new iterate(a)
	set1.insert('m');// new iterate(m)
	set1.insert('m');// new iterate(m)
	set1.insert('e');// new iterate(e)
	set1.insert('a');// new iterate(e)
	
	std::list<char> list1;// = "game";
	std::list<char> list3;// = "game";
	list1.push_back('g');// new iterate(g)
	list1.push_back('a');// new iterate(a)
	list1.push_back('m');// new iterate(m)
	list1.push_back('e');// new iterate(e)
	list1.push_back('a');// new iterate(e)
	//list1.~list<char>();
	//list1.clear();

	int iNum = list1.size();
	int iACnt = count(	list1.begin(), 
						list1.end(), 
						'a');	

	TCheck<char> check;
	check.cCompare = 'a';
	std::list<char>::iterator iFind = find_if(list1.begin(),
			list1.end(), check);
	
	/*list1.remove_if(IsFuction<char>());
	std::list<char>::iterator idata = 	 remove_if(list1.begin(), list1.end(), 
			 IsFuction<char>());

	for (std::list<char>::iterator itor = list1.begin();
	itor != idata;
		itor++)
	{
		std::cout << *itor << " ";
	}
	std::cout << std::endl;*/

	list3.assign(list1.begin(), list1.end());

	if (list3.empty() == true )
	{
		list3 = list1;
	}

	std::reverse(list3.begin(), list3.end());

	list1.remove('a');
	list1.sort();
	list1.unique();

	for (std::list<char>::iterator itor = list1.begin();
	     itor != list1.end();
		 itor++)
	{
		std::cout << *itor << " ";
	}
	std::cout << std::endl;

	//std::list<char>::iterator iterFind = std::find(list1.begin(), list1.end(), 'm');
	//
	//// list1의 반복자 iterFind 앞쪽에
	//// list3의 내용을 삽입하고
	//// iist3의 담긴 원소를 삭제한다.
	//list1.splice(iterFind, list3);
	//// 삭제
	//list1.erase(iterFind);

	//for (std::list<char>::iterator itor = list1.begin();
	//itor != list1.end();
	//	itor++)
	//{
	//	std::cout << *itor << " ";
	//}
	//std::cout << std::endl;

	//std::list<int> list2;// = "game";
	//list2.push_back(9000);// new iterate(g)
	//list2.push_back(11);// new iterate(a)
	//list2.push_back(3333);// new iterate(m)
	//list2.push_back(6);// new iterate(e)

	//for (std::list<int>::iterator itor = list2.begin();
	//	  itor != list2.end();	itor++)
	//{
	//	std::cout << *itor << " ";
	//}
	//	  std::cout << std::endl;
    return 0;
}

