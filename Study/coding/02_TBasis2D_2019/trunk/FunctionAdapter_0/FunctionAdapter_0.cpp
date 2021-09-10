// FunctionAdapter_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <functional> // bind2nd
using namespace std;

template <class T>
inline void PRINT(const T& coll, const char* optcstr = "")
{
	typename T::const_iterator pos;

	std::cout << optcstr;
	for (pos = coll.begin(); pos != coll.end(); ++pos)
	{
		std::cout << *pos << ' ';
	}
	std::cout << std::endl;
}
class IntSequence
{
private:
	int value;
public:
	// constructor
	IntSequence(int initialValue) : value(initialValue)
	{
	}

	// ``function call''
	int operator() ()
	{
		return value++;
	}
};
int main()
{
	list<int> coll;
	// insert values from 1 to 9
	generate_n(back_inserter(coll),    // start
		9,                      // number of elements
		IntSequence(1));        // generates values

	PRINT(coll);

	// 함수어댑터 bind2nd -> 1인자=계속 변경, 2인자=고정인자일 때 사용됨.
	// 함수어댑터 bind1st -> 1인자=고정, 2인자=계속변경 될 때 사용됨.
	list<int>::iterator itor = 
		find_if(coll.begin(), coll.end(), bind2nd(greater<int>(), 5));
	std::cout << *itor << endl;
	PRINT(coll);

	// 모든 홀수
	itor =
		find_if(coll.begin(), coll.end(), bind2nd(modulus<int>(), 2));
	std::cout << *itor << endl;
	PRINT(coll);
    return 0;
}

