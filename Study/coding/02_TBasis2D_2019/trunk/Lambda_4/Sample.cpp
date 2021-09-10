#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class TClass
{
public:
	float m_fScale = 2.0f;

	// capture "this" by reference
	void ApplyScale(const vector<int>& v) 
	{
		for_each(v.begin(), v.end(),
			[this](int n) { m_fScale = 5; cout << n * m_fScale << endl; });
	}

	// capture "this" by value (Visual Studio 2017 version 15.3 and later)
	// 속성지정 : 프로젝트 속성 -> C++ ->언어->C++ 언어표준-> "ISO C++17 표준(/ std:c++17)" 
	void ApplyScale2(const vector<int>& v)
	{
		for_each(v.begin(), v.end(),
			[*this](int n) { /*m_fScale = 5;*/ cout << n * m_fScale << endl; });
	}
	void ApplyScale3(const vector<int>& v) 
	{
		for_each(v.begin(), v.end(), 
			[=](int n) { m_fScale = 5; cout << n * m_fScale << endl; });
	}
	

};
void main()
{
	TClass t;
	std::vector<int> v = { 1,2,3,4,5 };
	
	t.ApplyScale3(v);
	t.ApplyScale(v);	
	t.ApplyScale2(v);

	auto p = make_unique<int[]>(5);	
	for (int i = 0; i < 5; ++i)
	{
		p[i] = i;
	}
	auto lambda = [ptr = move(p)](int n) { 
		for (int i = 0; i < n; ++i)
		{
			ptr[i] += 10;
			std::cout << ptr[i] << std::endl;
		}		
	};
	lambda(5);

	////
	int n = [](int x, int y) { return x + y; }(5, 4);
	cout << n << endl;
	////
	int i = 3;
	int j = 5;
	function<int(void)> f = [i, &j] { return i + j; };
	i = 22;
	j = 44;
	cout << f() << endl;
	////
	int timestwoplusthree = [](int x){return [](int y){ return y * 2; }(x)+3;}(5);
	cout << timestwoplusthree << endl;
	//
	//////  /EHsc /W4
	auto addtwointegers = [](int x) -> function<int(int)> 
	{	
		return [=](int y) 
		{ 
			return x + y; // 7+8
		};
	};
	auto higherorder = [](const function<int(int)>& f, int z) 
	{
		return f(z) * 2;
	};
	auto answer = higherorder(addtwointegers(7), 8);
	cout << answer << endl;//30
}