#include "namespaces.h"
#include <array>
#include <string>
#include <exception>
#include <stdexcept>
#include <cstddef>
#include <cstring>
using namespace mycode;
using namespace std;

// 새로운 함수 정의 문법, auto(타입추론)
auto func(int i) -> int
{
	return i + 2;
}

void func1(char* str) { cout << "char* version" << endl; }
void func2(int i) { cout << "int version" << endl; }

double divideNumbers(double numerator, double denominator)
{
	if (denominator == 0) {
		throw std::invalid_argument("Denominator cannot be 0.");
	}
	return numerator / denominator;
}



char* copyString(const char* str)
{
	//char* result = new char[strlen(str)];  // BUG! Off by one!
	char* result = new char[strlen(str) + 1];
	strcpy(result, str);
	return result;
}

char* appendStrings(const char* str1, const char* str2, const char* str3)
{
	char* result = new char[strlen(str1) + strlen(str2) + strlen(str3) + 1];
	strcpy(result, str1);
	strcat(result, str2);
	strcat(result, str3);
	return result;
}

int main()
{
	////////////////////////
	// namespace
	mycode::foo();	// Calls the "foo" function in the "mycode" namespace
	foo();			// implies mycode::foo();

					////////////////////////
					// iostream -> namespace std
	int uninitializedInt;
	int initializedInt = 7;
	// uninitializedInt는 초기화 하지 않으면 오류가 된다.
	uninitializedInt = 8;
	// 이전 버전에서는 초기화 되지 못한 변수를 출력하면
	// 쓰레기 값이 출력되지만 현재는 오류가 된다.
	//cout << uninitializedInt << " is a random value" << endl;
	cout << initializedInt << " was assigned an initial value" << endl;

	////////////////////////
	// typetest
	int someInteger = 256;
	short someShort;
	long someLong;
	float someFloat;
	double someDouble;

	someInteger++;
	someInteger *= 2;
	someShort = static_cast<short>(someInteger);
	someLong = someShort * 10000;
	someFloat = someLong + 0.785f;
	someDouble = static_cast<double>(someFloat) / 100000;

	cout << someDouble << endl;


	////////////////////////
	if (MyEnumData::EnumData1 == 0)
	{
		cout << "정수형 타입으로 자동 변환되기 때문에 오류가 아니다.";
	}
	////////////////////////
	//strongly Typed enums
	MyEnum value1 = MyEnum::EnumValue1;
	// bug
	/*if (MyEnum::EnumValue3 == 11)
	{
	cout << "문법오류";
	}*/

	////////////////////////
	// 구조체
	Employee anEmployee;

	anEmployee.firstInitial = 'M';
	anEmployee.lastInitial = 'G';
	anEmployee.employeeNumber = 42;
	anEmployee.salary = 80000;

	// output the values of an employee
	cout << "Employee: " << anEmployee.firstInitial <<
		anEmployee.lastInitial << endl;
	cout << "Number: " << anEmployee.employeeNumber << endl;
	cout << "Salary: $" << anEmployee.salary << endl;

	////////////////////////
	// 구간지정루프
	int arrayData[] = { 1,2,3,4 };
	for (auto& i : arrayData)
	{
		i += 2;
		cout << i;
	}
	////////////////////////
	// array	
	array<int, 3> arr = { 9, 8, 7 };
	cout << "Array size = " << arr.size() << endl;
	cout << "Element 2 = " << arr[1] << endl;

	//////////////////////// 
	// AlternativeFunctionSyntax(전통적이지 않은 함수 문법)
	cout << func(3) << endl;

	////////////////////////
	// NullPointerConstant NULL= 0이 된다.
	func2(NULL);
	func2(4);

	// 다음 문장은 오류가 된다.
	//func1(NULL);
	func1(nullptr);

	///////////////////////
	// string
	string myString = "Hello, World";
	cout << "The value of myString is " << myString << endl;


	///////////////////////
	// Exception
	try {
		std::cout << divideNumbers(2.5, 0.5) << std::endl;
		std::cout << divideNumbers(2.3, 0) << std::endl;
		std::cout << divideNumbers(4.5, 2.5) << std::endl;
	}
	catch (const std::exception& exception) {
		std::cout << "Exception caught: " << exception.what() << std::endl;
	}


	//////////////////////////
	//
	const char* str1 = "Hello";
	char* copy = copyString(str1);
	cout << copy << endl;
	delete[] copy;
	copy = nullptr;

	const char* str2 = " World";
	const char* str3 = "!";
	char* result = appendStrings(str1, str2, str3);
	cout << result << endl;
	delete[] result;
	result = nullptr;


	///////////////////////////
	//
	char text1[] = "abcdef";
	size_t s1 = sizeof(text1);  // is 7
	size_t s2 = strlen(text1);  // is 6
	cout << s1 << endl << s2 << endl;

	const char* text2 = "abcdef";
	size_t s3 = sizeof(text2);  // is platform-dependent (e.g. 4 bytes for x86, 8 bytes for x64)
	size_t s4 = strlen(text2);  // is 6
	cout << s3 << endl << s4 << endl;


	/////////////////////////////
	//
	string myString2 = "hello";

	myString2 += ", there";

	string myOtherString = myString2;

	if (myString == myOtherString) {
		myOtherString[0] = 'H';
	}

	cout << myString2 << endl;
	cout << myOtherString << endl;

	///////////////////////////////
	//
	//string str4 = "Hello "World"!";    // Error!
	string str4 = "Hello \"World\"!";
	string str5 = R"(Hello "World"!)";

	/*
	string str4 = "Line 1
	Line 2 with \t";    // Error!
	*/

	string str6 = R"(Line 1 Line 2 with \t)";

	//string str7 = R"(The characters )" are embedded in this string)";    // Error!
	string str7 = R"-(The characters )" are embedded in this string)-";
	return 0;
}
// 
//auto main() -> int
//{
//	cout << func(3) << endl;
//
//	return 0;
//}