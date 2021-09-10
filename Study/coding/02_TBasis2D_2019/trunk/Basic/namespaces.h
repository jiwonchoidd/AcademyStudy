#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//#pragma warning (disable:4700)            // 경고를 무시하도록 한다.
//#pragma warning (once:4700)          // 4700경고를 한 번만 출력한다.
//#pragma warning (error:4700)               // 경고 대신 에러를 출력한다.
//#pragma warning (3:4700)                // 4700번 경고를 레벨 3으로 올린다.
namespace mycode {
	void foo();
}
enum MyEnumData
{
	EnumData1,
	EnumData2 = 10,
	EnumData3,
};
// 엄격한 열거 타입
enum class MyEnum
{
	EnumValue1,
	EnumValue2 = 10,
	EnumValue3
};
// 기본 int 형에서 long 형으로 타입을 변경한다.
enum class MyEnumLong : unsigned long
{
	EnumValueLong1,
	EnumValueLong2 = 10,
	EnumValueLong3
};

struct Employee {
	char firstInitial;
	char lastInitial;
	int  employeeNumber;
	int  salary;
};


