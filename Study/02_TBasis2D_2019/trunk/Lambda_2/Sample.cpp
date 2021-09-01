#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <list>
#include <algorithm> //for_each
using namespace std;

template <class T>
class generator
{
	T val_;
public:
	generator(const T &val) : val_(val) { }
	T operator()() {
		return val_ += val_;
	}
};

function<int(void)> multiplyBy2Lambda(int x)
{
	return [x] { return 2 * x; };
}

// Using C++14 function return type deduction
//auto multiplyBy2Lambda(int x)
//{
//	return [x]{ return 2 * x; };
//}
void Print(int i)
{
	std::cout << i << " " << std::endl;
}
class TClass {};
int main()
{
	//function<int(void)> fn = multiplyBy2Lambda(5);
	//cout << fn() << endl;

	auto fn = multiplyBy2Lambda(5);
	cout << fn() << endl;

	constexpr int n = 10;
	std::vector<TClass*> v1(n);
	std::generate(v1.begin(), v1.end(), []() { return new TClass(); });


	std::vector<TClass*> v2;
	v2.reserve(n); 
	std::generate_n(std::back_inserter(v2), n, [] { return new TClass(); });

	typedef std::vector<int,	std::allocator<int> > kVector;
	generator<kVector::value_type> gen(1);
	std::vector<int> v3;
	std::generate_n(std::back_inserter(v3), 5, gen);
	std::generate(v3.begin(), v3.end(), [n = 0]() mutable { return n++; });
	auto delList = [](auto& i) { Print(i); };
	std::for_each(v3.begin(), v3.end(), delList);

	return 0;
}