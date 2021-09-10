// 0_StronglyTypedEnums.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <array> // std::array
// 열거형은 기본적으로 정수 타입 취급을 받기 때문에 서로 별개인
// 열거 타입 간에 비교 연산이 가능하다. 그래서 이 문제를 해결하려고
//enum class를 도입하였다.
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
	cout << __func__ << "에서 반환된 값 출력 = "<< endl;
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
	// 구간 지정 루프
	int arr[] = { 0,1,2,3,4 };
	for (auto& iData : arr)
	{
		std::cout << iData;
	}
	// std::array
	// 단점 : 배열과 동일하다고 보면 된다. 컴파일 시점에서 크기가 고정되어
	// 실행 시점에는 크기를 키우거나 줄일 수 없다.
	array<int, 5> arr2 = { 0,1,2,3,4 };
	cout << arr2.size() << endl;
	for (auto& iData : arr2)
	{
		std::cout << iData;
	}
	// \n 줄바꿈, \r 출력 커서를 처음으로 옭김(캐리지 리턴)
	cout << '\r' << '\n';
	cout << __func__ << "함수에서 호출되어 ";
	cout << addNumbers(10, 20);
    return 0;
}

