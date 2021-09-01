// 배열과 유사하다.( 템플릿 클래스 )
//https://msdn.microsoft.com/ko-kr/library/ct1as7hw.aspx
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <iterator>
#include <random>
#include <functional>
using namespace std;

template <typename T > vector<char> make(char s[])
{
	return T(&s[0], &s[strlen(s)]);
}

void main()
{	
	// T ==> vector<char>
	vector<char> a, b(3, 'x');
	assert(a.size() == 0);
	assert(b.size() == 3);
	for (int i = 0; i < b.size(); i++)
	{
		cout << b[i] << endl;
	}
	assert(b[0] == 'x' && b[1] == 'x'&&	b[2] == 'x');
	assert(b == vector<char>(3, 'x'));


	vector<char> vector1 = make<vector<char>>("Bjarne Struoustrup");
	vector<char> vector2;
	vector<char>::iterator i;
	for (i = vector1.begin(); i != vector1.end(); ++i)
	{
		//vector2.push_back(*i);
		vector2.insert(vector2.begin(), *i);
	}
	reverse(vector2.begin(), vector2.end());
	while (vector2.size() > 0)
	{
		cout << vector2.front();
		vector2.erase(vector2.begin());		
	}


	char name[] = "kgca game academy";
	vector<char>  data(name, name+strlen(name));
	vector<char>  copydata(data.begin(),data.end());
	assert(data == copydata);
	vector<char> aaaa = copydata;	

	//std::copy(aaaa.begin(), aaaa.end(), data.begin());
	// data 뒤에 복사한다.
	copy(aaaa.begin(), aaaa.end(), back_inserter(data));

	vector<int> vector3(10);
	// 연결리스트 : 연속적 메모리를 사용
	for (int i = 0; i < vector3.size(); i++)
	{		
		vector3[i]=i;
	}		
	for (	std::vector<int>::reverse_iterator iter = vector3.rbegin();
			iter != vector3.rend();
			iter++)
	{		
		std::cout << *iter << std::endl;
	}	
	for (int i = 0; i < vector3.size(); i++)
	{
		std::cout << vector3[i] << std::endl;
	}
	vector3.clear();


	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> dis(0, 9);

	std::vector<int> v1(10), v2(10);
	std::generate(v1.begin(), v1.end(), std::bind(dis, std::ref(mt)));
	std::generate(v2.begin(), v2.end(), std::bind(dis, std::ref(mt)));

	// sort
	std::sort(v1.begin(), v1.end());
	std::sort(v2.begin(), v2.end());

	// output v1
	std::cout << "v1 : ";
	std::copy(v1.begin(), v1.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';

	// output v2
	std::cout << "v2 : ";
	std::copy(v2.begin(), v2.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';

	// merge
	std::vector<int> dst;
	std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(dst));

	// output
	std::cout << "dst: ";
	std::copy(dst.begin(), dst.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';
}
