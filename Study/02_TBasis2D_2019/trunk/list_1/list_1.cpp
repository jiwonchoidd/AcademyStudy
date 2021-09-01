// list_1.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"
#include <list>
#include <iostream>
using namespace std;

struct Tdata
{
	int    iValue;
	float  fValue;
	int*   ipArray;
};
int main()
{
	list<Tdata*> list1;
	for (int i = 0; i < 10; i++ )
	{
		Tdata* a = new Tdata;
		a->iValue = i;
		a->fValue = (float)i;
		a->ipArray = new int[3];
		a->ipArray[0] = 100;
		a->ipArray[1] = 200;
		a->ipArray[2] = 300;
		list1.push_back(a);
	}
	for (std::list<Tdata*>::iterator itor = list1.begin();itor != list1.end();)
	{
		Tdata* data = *itor;
		std::cout << data->iValue << data->fValue << " " << data->ipArray[0] << " ";
		// list�� ���� ó�� ���ǿ� itor++ or ++itor�ϸ� ������ �ȴ�.
		//list1.erase(itor++);	
		//itor = list1.erase(itor);
	}
	std::cout << std::endl;

	for (std::list<Tdata*>::iterator itor = list1.begin();
		itor != list1.end();
		itor++)
	{
		Tdata* data = *itor;
		delete[] data->ipArray;
		delete data;
		list1.erase(itor);
	}
	list1.clear();

	
	list<Tdata> list2;
	for (int i = 0; i < 10; i++)
	{
		Tdata a ;
		a.iValue = i;
		a.fValue = (float)i;
		a.ipArray = new int[3];
		a.ipArray[0] = 100;
		a.ipArray[1] = 200;
		a.ipArray[2] = 300;
		list2.push_back(a);
	}
	list<Tdata> list3(list2.begin(), list2.end());
	for (std::list<Tdata>::iterator itor = list3.begin();
	itor != list3.end();
		itor++)
	{
		Tdata data = *itor;
		data.ipArray[0] = 9999;
		std::cout << data.iValue << data.fValue << " ";
	}
	std::cout << std::endl;
	
	list2.clear();
	list3.clear();
    return 0;
}

