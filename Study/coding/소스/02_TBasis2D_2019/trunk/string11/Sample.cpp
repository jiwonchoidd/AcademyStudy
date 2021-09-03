#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
using namespace std;

string Get03(vector<std::string>& v)
{
	std::string s;
	for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
		s += *i;
	return s; 
}
string Get11(vector<std::string>& v)
{
	std::string s;
	for_each(v.begin(), v.end(), [&](const std::string &piece) { s += piece; });
	return s;
}
string Get(vector<std::string>& v)
{
	std::string s;
	for (const auto &piece : v) s += piece;
	return s;
}
void main()
{
	std::vector<std::string> v{ "Hello, ", " KGCA ", "GameAcademy!" };
	std::string s;
	s = accumulate(begin(v), end(v), s);
	std::cout << s; // "Hello, KGCA GameAcademy!"

	std::string result;
	for (auto const& s : v) { result += s; }
	std::cout << result; // "Hello, KGCA GameAcademy!"
		
	std::string s5 = std::accumulate(v.begin(), v.end(), std::string{},
		[](std::string &s, const std::string &piece) -> decltype(auto) { return s += piece; });

	std::cout << Get(v);
	std::cout << Get03(v);
	std::cout << Get11(v);
	// "Hello, KGCA GameAcademy!"
}