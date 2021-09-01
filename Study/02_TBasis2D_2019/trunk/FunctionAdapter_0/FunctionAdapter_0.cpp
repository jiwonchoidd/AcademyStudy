// FunctionAdapter_0.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

	// �Լ������ bind2nd -> 1����=��� ����, 2����=���������� �� ����.
	// �Լ������ bind1st -> 1����=����, 2����=��Ӻ��� �� �� ����.
	list<int>::iterator itor = 
		find_if(coll.begin(), coll.end(), bind2nd(greater<int>(), 5));
	std::cout << *itor << endl;
	PRINT(coll);

	// ��� Ȧ��
	itor =
		find_if(coll.begin(), coll.end(), bind2nd(modulus<int>(), 2));
	std::cout << *itor << endl;
	PRINT(coll);
    return 0;
}

