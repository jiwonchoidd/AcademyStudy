#pragma warning(disable: 4996)
#include <iostream>
#include <string>
int main()
{
	const char* cc = "a";
	std::string str;
	str = cc;
	std::cout << "str=" << str.data() << " 문자열 크기" << str.length() << std::endl;
	str = "클래스 내부에 공간 잡음";
	str += ".";
	std::cout << "str=" << str.data() << "문자열크기" << str.length() << std::endl;
}