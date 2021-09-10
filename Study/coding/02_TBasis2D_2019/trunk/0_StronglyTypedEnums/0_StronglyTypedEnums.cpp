// 0_StronglyTypedEnums.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
#include <array> // std::array
// �������� �⺻������ ���� Ÿ�� ����� �ޱ� ������ ���� ������
// ���� Ÿ�� ���� �� ������ �����ϴ�. �׷��� �� ������ �ذ��Ϸ���
//enum class�� �����Ͽ���.
using namespace std;

enum class MyEnum
{
	EnumValue1,
	EnumValue2 = 10,
	EnumValue3,
};
enum class MyEnumLong : unsigned long
{
	EnumValueLong1,
	EnumValueLong2 = 10,
	EnumValueLong3,
};

int addNumbers(int i, int j)
{
	cout << __func__ << "���� ��ȯ�� �� ��� = "<< endl;
	return  i + j;
}
int main()
{
	/*if (MyEnum::EnumValue2 == 10) // Error
	{
		std::cout << (int)MyEnum::EnumValue2;
	}*/
	MyEnum my;
	my = MyEnum::EnumValue2;
	if (MyEnum::EnumValue2 == my)
	{
		std::cout << (int)MyEnum::EnumValue2 << std::endl;
	}
	// ���� ���� ����
	int arr[] = { 0,1,2,3,4 };
	for (auto& iData : arr)
	{
		std::cout << iData;
	}
	// std::array
	// ���� : �迭�� �����ϴٰ� ���� �ȴ�. ������ �������� ũ�Ⱑ �����Ǿ�
	// ���� �������� ũ�⸦ Ű��ų� ���� �� ����.
	array<int, 5> arr2 = { 0,1,2,3,4 };
	cout << arr2.size() << endl;
	for (auto& iData : arr2)
	{
		std::cout << iData;
	}
	// \n �ٹٲ�, \r ��� Ŀ���� ó������ �ı�(ĳ���� ����)
	cout << '\r' << '\n';
	cout << __func__ << "�Լ����� ȣ��Ǿ� ";
	cout << addNumbers(10, 20);
    return 0;
}

