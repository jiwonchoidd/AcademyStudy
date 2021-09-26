#include <iostream>
#include <vector>
#include <string>
int main()
{
	//https://www.acmicpc.net/problem/1032
	int iName_Amount=0;
	std::cin >> iName_Amount;
	std::vector<std::string> vName;
	for (int i = 0; i < iName_Amount; i++)
	{
		std::string tempname="";
		std::cin >> tempname;
		vName.push_back(tempname);
	}

	for (std::vector<std::string>::iterator iter = vName.begin();
		iter != vName.end();
		iter++)
	{
		//최대 길이를 받고
		//점이 있는 

	}
}