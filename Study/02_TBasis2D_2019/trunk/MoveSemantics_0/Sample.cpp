#include <iostream>
#include <vector>
#include "Spreadsheet.h"

using namespace std;

Spreadsheet CreateObject()
{
	return Spreadsheet(3, 2);
}
// 복제한다.
template <typename T>
void Swap(T& a, T& b)
{
	T temp(a);
	a = b; 
	b = temp;
}
// 이동한다. 
//이동 시맨틱은 원본 객체가 삭제되리라는 것을 알 때만 유용하게 활용할 수 있다.
template <typename T>
void SwapMove(T& a, T& b)
{
	T temp(std::move(a));
	a = std::move(b);
	b = std::move(temp);
}
int main()
{
	int a = 10;
	int b = 20;
	Swap<int>(a, b);
	int c = 10;
	int d = 20;
	SwapMove<int>(c, d);

	vector<Spreadsheet> vec;
	for (int i = 0; i < 2; ++i) {
		cout << "Iteration " << i << endl;
		vec.push_back(Spreadsheet(100, 100));
		cout << endl;
	}

	Spreadsheet s(2, 3);
	// 이동 대입 연산자 호출
	// 이유는 CreateObject함수가 반환후에 임시객체이기 때문에 소멸한다.
	s = CreateObject();

	Spreadsheet s2(5, 6);
	// 일반 복제 대입연산자 호출
	s2 = s;

	return 0;
}
