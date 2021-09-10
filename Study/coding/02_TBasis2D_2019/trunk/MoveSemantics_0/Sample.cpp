#include <iostream>
#include <vector>
#include "Spreadsheet.h"

using namespace std;

Spreadsheet CreateObject()
{
	return Spreadsheet(3, 2);
}
// �����Ѵ�.
template <typename T>
void Swap(T& a, T& b)
{
	T temp(a);
	a = b; 
	b = temp;
}
// �̵��Ѵ�. 
//�̵� �ø�ƽ�� ���� ��ü�� �����Ǹ���� ���� �� ���� �����ϰ� Ȱ���� �� �ִ�.
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
	// �̵� ���� ������ ȣ��
	// ������ CreateObject�Լ��� ��ȯ�Ŀ� �ӽð�ü�̱� ������ �Ҹ��Ѵ�.
	s = CreateObject();

	Spreadsheet s2(5, 6);
	// �Ϲ� ���� ���Կ����� ȣ��
	s2 = s;

	return 0;
}
