// String_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <algorithm> // transform
using namespace std;
bool nocase_compare(char c1, char c2)
{
	return toupper(c1) == toupper(c2);
}
int main()
{
	string string1 = "kgca";
	std::cout << string1.c_str() << endl;

	wstring string2 = L"kgca";
	std::wcout << string2.c_str() << endl;

	string str1 = "Good ";
	string str2 = "morning";
	string str3 = str1 + str2;

	cout << str1 << endl;
	cout << str2 << endl;
	cout << str3 << endl;

	str1 += str2;
	if (str1 == str3) {   //str1과 str3의 내용 비교.	
		cout << "equal!" << endl;
	}

	string str4;
	cout << "문자열 입력: ";
	cin >> str4;
	cout << "입력한 문자열: " << str4 << endl;


	// create a string
	string s("The zip code of Hondelage in Germany is 38108");
	cout << "original: " << s << endl;

	// lowercase all characters
	transform(s.begin(), s.end(),    // source
		s.begin(),             // destination
		tolower);              // operation
	std::cout << "lowered:  " << s << endl;

	// uppercase all characters
	transform(s.begin(), s.end(),    // source
		s.begin(),             // destination
		toupper);              // operation
	std::cout << "uppered:  " << s << endl;

	///////////////////////////////// equal /////////////////////////////
	string s1("This is a string");
	string s2("STRING");

	// compare case insensitive
	if (s1.size() == s2.size() &&        // ensure same sizes
		equal(s1.begin(), s1.end(),      // first source string
			s2.begin(),               // second source string
			nocase_compare)) {        // comparison criterion
		std::cout << "the strings are equal" << endl;
	}
	else {
		std::cout << "the strings are not equal" << endl;
	}

	// search case insensitive
	string::iterator pos;
	pos = search(s1.begin(), s1.end(),   // source string in which to search
		s2.begin(), s2.end(),   // substring to search
		nocase_compare);       // comparison criterion
	if (pos == s1.end()) {
		std::cout << "s2 is not a substring of s1" << endl;
	}
	else {
		std::cout << '"' << s2 << "\" is a substring of \""
			<< s1 << "\" (at index " << pos - s1.begin() << ")"
			<< endl;
	}

	/////////////////////////////// sort & erase //////////////////////////
	// create constant string
	const string hello("Hello, how are you?");

	// initialize string s with all characters of string hello
	string s5(hello.begin(), hello.end());

	// iterate through all of the characters

	for (string::iterator pos = s5.begin(); pos != s5.end(); ++pos) {
		cout << *pos;
	}
	std::cout << endl;

	// reverse the order of all characters inside the string
	reverse(s5.begin(), s5.end());
	std::cout << "reverse:       " << s5 << endl;

	// sort all characters inside the string
	sort(s5.begin(), s5.end());
	std::cout << "ordered:       " << s5 << endl;

	/* remove adjacent duplicates
	* - unique() reorders and returns new end
	* - erase() shrinks accordingly
	*/
	s5.erase(unique(s5.begin(),	s5.end()),
		s5.end());
	std::cout << "no duplicates: " << s5 << endl;
    return 0;
}

