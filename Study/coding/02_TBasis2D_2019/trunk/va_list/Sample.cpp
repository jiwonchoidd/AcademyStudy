#include <iostream> //initializer_list , vfprintf
#include <initializer_list>//initializer_list
#include <cstdarg> //va_list
using namespace std;

bool debug = false;

#pragma region C
void debugOut(char* str, ...)
{
	va_list ap;
	if (debug)
	{
		va_start(ap, str);
		//vfprintf or vprintf를 사용해야 한다.
		// printf함수를 사용하면 안됨.
		vfprintf(stderr, str, ap);
		va_end(ap);
	}
}
// 나열 연산자는 데이터의 통일.
// 첫번째 인자값 반드시 사용해야 한다.
void printInts(int num, ...)
{
	int temp;
	va_list ap;
	va_start(ap, num);
	for (int i = 0; i < num; i++)
	{
		temp = va_arg(ap, int);
		cout << temp << " ";
	}
	va_end(ap);
	cout << endl;
}
#pragma endregion
#pragma region C++11
//int만 초기화 된다.
int makeSum(initializer_list<int> lst) 
{
	int total = 0;
	for (auto iter = lst.begin(); iter != lst.end(); ++iter)
		total += (*iter);
	return total;
}

#pragma endregion
void main()
{
	debug = true;
	debugOut("int %d\n", 5);
	debugOut("String %s and int %d\n", "hello",5);
	debugOut("Many ints : %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5);

	printInts(5, 1, 2, 3, 4, 5);

	int a = makeSum({ 1,2,3,4,5 });
	//int b = makeSum({ 1,2,3, 3.0 })  // 한가지 타입만 가능하기 때문에 오류다.
}