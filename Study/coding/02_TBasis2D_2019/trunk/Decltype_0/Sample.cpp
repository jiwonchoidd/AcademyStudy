#include <iostream>
#include <vector>
using namespace std;

int Multiply(int a, int b) { return a * b; };

template <typename T1, typename T2>
auto add(T1 first, T2 second) -> decltype(first + second) {
	return first + second;
}

class class_c {
public:
	int max;
	int min;
	int middle;

	class_c() {}
	class_c(int my_max) {
		max = my_max > 0 ? my_max : 10;
	}
	class_c(int my_max, int my_min) {
		max = my_max > 0 ? my_max : 10;
		min = my_min > 0 && my_min < max ? my_min : 1;
	}
	class_c(int my_max, int my_min, int my_middle) {
		max = my_max > 0 ? my_max : 10;
		min = my_min > 0 && my_min < max ? my_min : 1;
		middle = my_middle < max && my_middle > min ? my_middle : 5;
	}
};
class class_d {
public:
	int max;
	int min;
	int middle;

	class_d(int my_max) {
		max = my_max > 0 ? my_max : 10;
	}
	class_d(int my_max, int my_min) : class_d(my_max) {
		min = my_min > 0 && my_min < max ? my_min : 1;
	}
	class_d(int my_max, int my_min, int my_middle) : class_d(my_max, my_min) {
		middle = my_middle < max && my_middle > min ? my_middle : 5;
	}
};
class NonCopyClass {
public:
	int iValue;
public:
	NonCopyClass() = default;
	NonCopyClass& operator =(const NonCopyClass&) = delete;
	NonCopyClass(const NonCopyClass&) = delete;
};

int onlyDouble(double value) { return value* value; }
template <typename T> void onlyDouble(T) = delete;

class Base {
	virtual void func1() {};
	virtual void func2(float) {};
	virtual void func3() const {};
	virtual long func4(int ivalue) { return ivalue; };
};
class Derived : public Base {
	// Base�� ���� �Լ�
	//virtual void fun1() override {}; // ERROR
	// ���ڰ��� �ٸ� �Լ�
	//virtual void func2(double) override {}; // ERROR
	// const�� ���� �Լ�
	//virtual void func3()  override {}; // ERROR
	// ��ȯ���� �ٸ� �Լ�
	//virtual int func4(int ivalue) override { return ivalue; }; // ERROR
	// Base�� �ִ� �Լ�(����, ��ȯ)
	virtual long func4(int ivalue) override { return ivalue; }; // OK
	// Base�� ���� �Լ�
	//void fun2() override {};
};


void main()
{
	cout << onlyDouble(3.0) << endl; // ���� double�� ���Ǿ� �Ѵ�.
	//cout << onlyDouble(3) << endl; // Error ���� ���Ǿ��� ������

	NonCopyClass a;
	a.iValue = 10;
	//NonCopyClass b = a; // Error! ���� ����
	//NonCopyClass& c = a;// Error! ���� ����

	class_d d{ 1,3,2 };
	cout << d.max << " " << d.middle << " "	<< d.min << endl;
#pragma region 0
	auto myString = "my String"; // C++11
	auto myInt = 5; // C++11
	auto myDouble = 3.14; // C++11
	cout << myString << endl; // 2
	cout << myInt << endl; // 2
	cout << myDouble << endl; // 2
#pragma endregion
#pragma region 1
	decltype("str") myString2 = "str"; // C++11
	decltype(5) myInt2 = 5; // C++11
	decltype(3.14) myFloat2 = 3.14; // C++11
	decltype(myInt) myNewInt2 = 2011; // C++11
	cout << myString2 << endl; // 2
	cout << myInt2 << endl; // 2
	cout << myFloat2 << endl; // 2
	cout << myNewInt2 << endl; // 2
#pragma endregion
#pragma region 2
	vector<int> v(2);
	v[0] = 999; v[1] = 888;
	vector<int>::iterator it1 = v.begin(); // C++98
	auto it2 = v.begin(); // C++11
	cout << *it2 << endl; // 2
#pragma endregion
#pragma region 3
	int(*myAdd1)(int, int) = Multiply; // C++98
	auto myFuction = Multiply;
	cout << myAdd1(2,3) << " " << myFuction(2, 3)<< " " << Multiply(2, 3) << endl; // 2
#pragma endregion
#pragma region 4
	cout << add(1, 1) << endl; // 2
	cout << add(1, 1.1) << endl; // 2.1
	cout << add(1000LL, 5) << endl; // 1005
#pragma endregion
}