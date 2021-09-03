#include <iostream>
struct S 
{
	void operator()(int a, int b) { std::cout << "a + b = " << a + b << std::endl; }
};

int main() 
{
	S some_obj;
	some_obj(3, 5);
}
///////////////////////////////////////////////////////////
//#include <iostream>
//int main() 
//{
//	auto f = [](int a, int b) { std::cout << "a + b = " << a + b << std::endl; };
//	f(3, 5);
//}
/////////////////////////////////////////////////////////
//#include <functional>
//#include <iostream>
//#include <string>
//
//int some_func1(const std::string& a) 
//{
//	std::cout << "Func1 호출! " << a << std::endl;
//	return 0;
//}
//struct S 
//{
//	void operator()(char c) { std::cout << "Func2 호출! " << c << std::endl; }
//};
//int main() 
//{
//	std::function<int(const std::string&)> f1 = some_func1;
//	std::function<void(char)> f2 = S();
//	std::function<void()> f3 = []() { std::cout << "Func3 호출! " << std::endl; };
//
//	f1("hello");
//	f2('c');
//	f3();
//}
/////////////////////////////////////////////////////////
//#include <functional>
//#include <iostream>
//#include <string>
//
//class A {
//	int c;
//
//public:
//	A(int c) : c(c) {}
//	int some_func() { std::cout << "내부 데이터 : " << c << std::endl; }
//};
//
//int main() {
//	A a(5);
//	std::function<int()> f1 = a.some_func;
//}
//
/////////////////////////////////////////////////////////
//#include <functional>
//#include <iostream>
//#include <string>
//
//class A {
//	int c;
//
//public:
//	A(int c) : c(c) {}
//	int some_func() {
//		std::cout << "비상수 함수: " << ++c << std::endl;
//		return c;
//	}
//
//	int some_const_function() const {
//		std::cout << "상수 함수: " << c << std::endl;
//		return c;
//	}
//
//	static void st() {}
//};
//
//int main() {
//	A a(5);
//	std::function<int(A&)> f1 = &A::some_func;
//	std::function<int(const A&)> f2 = &A::some_const_function;
//
//	f1(a);
//	f2(a);
//}
/////////////////////////////////////////////////////////
//#include <algorithm>
//#include <functional>
//#include <iostream>
//#include <vector>
//using std::vector;
//
//int main() {
//	vector<int> a(1);
//	vector<int> b(2);
//	vector<int> c(3);
//	vector<int> d(4);
//
//	vector<vector<int>> container;
//	container.push_back(b);
//	container.push_back(d);
//	container.push_back(a);
//	container.push_back(c);
//
//	vector<int> size_vec(4);
//	std::transform(container.begin(), container.end(), size_vec.begin(),
//		&vector<int>::size);
//	for (auto itr = size_vec.begin(); itr != size_vec.end(); ++itr) {
//		std::cout << "벡터 크기 :: " << *itr << std::endl;
//	}
//}
//
/////////////////////////////////////////////////////////
//#include <algorithm>
//#include <functional>
//#include <iostream>
//#include <vector>
//using std::vector;
//
//int main() {
//	vector<int> a(1);
//	vector<int> b(2);
//	vector<int> c(3);
//	vector<int> d(4);
//
//	vector<vector<int>> container;
//	container.push_back(a);
//	container.push_back(b);
//	container.push_back(c);
//	container.push_back(d);
//
//	std::function<size_t(const vector<int>&)> sz_func = &vector<int>::size;
//
//	vector<int> size_vec(4);
//	std::transform(container.begin(), container.end(), size_vec.begin(), sz_func);
//	for (auto itr = size_vec.begin(); itr != size_vec.end(); ++itr) {
//		std::cout << "벡터 크기 :: " << *itr << std::endl;
//	}
//}
//
/////////////////////////////////////////////////////////
//#include <algorithm>
//#include <functional>
//#include <iostream>
//#include <vector>
//using std::vector;
//
//int main() {
//	vector<int> a(1);
//	vector<int> b(2);
//	vector<int> c(3);
//	vector<int> d(4);
//
//	vector<vector<int>> container;
//	container.push_back(a);
//	container.push_back(b);
//	container.push_back(c);
//	container.push_back(d);
//
//	vector<int> size_vec(4);
//	transform(container.begin(), container.end(), size_vec.begin(),
//		std::mem_fn(&vector<int>::size));
//	for (auto itr = size_vec.begin(); itr != size_vec.end(); ++itr) {
//		std::cout << "벡터 크기 :: " << *itr << std::endl;
//	}
//}
//
/////////////////////////////////////////////////////////
//#include <functional>
//#include <iostream>
//
//void add(int x, int y) {
//	std::cout << x << " + " << y << " = " << x + y << std::endl;
//}
//
//void subtract(int x, int y) {
//	std::cout << x << " - " << y << " = " << x - y << std::endl;
//}
//int main() {
//	auto add_with_2 = std::bind(add, 2, std::placeholders::_1);
//	add_with_2(3);
//
//	// 두 번째 인자는 무시된다.
//	add_with_2(3, 4);
//
//	auto subtract_from_2 = std::bind(subtract, std::placeholders::_1, 2);
//	auto negate =
//		std::bind(subtract, std::placeholders::_2, std::placeholders::_1);
//
//	subtract_from_2(3);  // 3 - 2 를 계산한다.
//	negate(4, 2);        // 2 - 4 를 계산한다
//}
/////////////////////////////////////////////////////////
//#include <functional>
//#include <iostream>
//
//struct S {
//	int data;
//	S(int data) : data(data) { std::cout << "일반 생성자 호출!" << std::endl; }
//	S(const S& s) {
//		std::cout << "복사 생성자 호출!" << std::endl;
//		data = s.data;
//	}
//
//	S(S&& s) {
//		std::cout << "이동 생성자 호출!" << std::endl;
//		data = s.data;
//	}
//};
//
//void do_something(S& s1, const S& s2) { s1.data = s2.data + 3; }
//
//int main() {
//	S s1(1), s2(2);
//
//	std::cout << "Before : " << s1.data << std::endl;
//
//	// s1 이 그대로 전달된 것이 아니라 s1 의 복사본이 전달됨!
//	auto do_something_with_s1 = std::bind(do_something, s1, std::placeholders::_1);
//	do_something_with_s1(s2);
//
//	std::cout << "After :: " << s1.data << std::endl;
//}
/////////////////////////////////////////////////////////
//#include <functional>
//#include <iostream>
//
//struct S {
//	int data;
//	S(int data) : data(data) { std::cout << "일반 생성자 호출!" << std::endl; }
//	S(const S& s) {
//		std::cout << "복사 생성자 호출!" << std::endl;
//		data = s.data;
//	}
//
//	S(S&& s) {
//		std::cout << "이동 생성자 호출!" << std::endl;
//		data = s.data;
//	}
//};
//
//void do_something(S& s1, const S& s2) { s1.data = s2.data + 3; }
//
//int main() {
//	S s1(1), s2(2);
//
//	std::cout << "Before : " << s1.data << std::endl;
//
//	// s1 이 그대로 전달된 것이 아니라 s1 의 복사본이 전달됨!
//	auto do_something_with_s1 =
//		std::bind(do_something, std::ref(s1), std::placeholders::_1);
//	do_something_with_s1(s2);
//
//	std::cout << "After :: " << s1.data << std::endl;
//}