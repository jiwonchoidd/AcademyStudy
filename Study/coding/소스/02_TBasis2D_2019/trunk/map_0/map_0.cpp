// map_0.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <map>
#include <string>
#include <iostream>
using namespace std;

struct tData
{
	int iValue;
};
int main()
{
	std::map< string, int > map1;
	map1["ȫ�浿"] = 1000;
	map1["��ȭ��"] = 2000;
	map1.insert(make_pair("ȫ�浿",1000));

	std::map< string, int >::iterator ITOR=
		map1.find("ȫ�浿");
	int iValue = ITOR->second;

	std::cout << map1["ȫ�浿"] << endl;
	std::cout << map1["��ȭ��"] << endl;
	//         Ű, ������
	int m_iIndex = 0;
	std::map< int, tData > map2;
	tData data0, data1;
	data0.iValue = 9999;
	data1.iValue = 7777;
	map2[0] = data0;
	map2[1] = data1;
	map2.insert(make_pair(m_iIndex++, data0));

	std::map< int, tData  >::iterator ITOR2 =
		map2.find(0);
	tData tValue = ITOR2->second;

	for (int i = 0; i < map2.size(); i++)
	{		
		std::cout << "key[" << i << "]" << ":" << 
			"data[" << map2[i].iValue << "]" << endl;
	}

	for (std::map<string, int>::iterator itor = map1.begin();
	itor != map1.end();
		itor++)
	{
		string key = itor->first;
		int    data = itor->second;
		std::cout << key << ":" << data << endl;
	}
	for (std::map<int, tData>::iterator itor = map2.begin();
	itor != map2.end();
		itor++)
	{
		int		 key = itor->first;
		tData    data = itor->second;
		std::cout << key << ":" << data.iValue << endl;
	}
	map1.clear();
    return 0;
}

