#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//#pragma warning (disable:4700)            // ��� �����ϵ��� �Ѵ�.
//#pragma warning (once:4700)          // 4700��� �� ���� ����Ѵ�.
//#pragma warning (error:4700)               // ��� ��� ������ ����Ѵ�.
//#pragma warning (3:4700)                // 4700�� ��� ���� 3���� �ø���.
namespace mycode {
	void foo();
}
enum MyEnumData
{
	EnumData1,
	EnumData2 = 10,
	EnumData3,
};
// ������ ���� Ÿ��
enum class MyEnum
{
	EnumValue1,
	EnumValue2 = 10,
	EnumValue3
};
// �⺻ int ������ long ������ Ÿ���� �����Ѵ�.
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


