#include <map>
#include <iostream>
int main()
{
	std::map<int, int> maplist;
	maplist[0] = 1000;
	std::map<int, int>::iterator iter = maplist.find(0);
	if (iter != maplist.end())
	{
		std::cout <<iter->second;
	}
}
