#include <map>
#include <iostream>
#include <string>
class THuman
{
public :
	int		iAge;
	std::wstring address;
	std::wstring tel;
public:
	void Set(int, std::wstring, std::wstring);
};
void THuman::Set(int a, std::wstring b, std::wstring c)
{
	iAge = a;
	address = b;
	tel = c;
}
int main()
{
	std::map<std::wstring, THuman*> maplist;
	THuman* p1 = new THuman;
	THuman* p2 = new THuman;
	THuman* p3 = new THuman;
	p1->Set(25,L"기산동",L"010");
	p2->Set(21, L"권선동", L"010");
	p3->Set(20, L"영통동", L"010");
	/*maplist[L"홍길동"] = h1;
	maplist[L"최지원"] = h2;
	maplist[L"이근영"] = h3;*/
	maplist.insert(std::make_pair(L"최지원", p1));
	maplist.insert(std::make_pair(L"강병주", p2));
	maplist.insert(std::make_pair(L"이근영", p3));
	std::map<std::wstring, THuman*>::iterator iter = maplist.find(L"홍길동");
	if (iter!= maplist.end())
	{
		std::wstring iKey = iter->first;
		THuman* data = iter->second;
		data->iAge = 99;
		std::wcout << iKey <<"";
		std::wcout << data->iAge<< "Hello World!\n";
		delete data;
		data = nullptr;
		maplist.erase(iter);
	}
	for (auto it = maplist.begin();
		it != maplist.end();
		it++)
	{
		THuman* data = it->second;
		delete data;
		it->second = nullptr;
	}
	maplist.clear();
	std::cout << "Hello World!\n";
}

