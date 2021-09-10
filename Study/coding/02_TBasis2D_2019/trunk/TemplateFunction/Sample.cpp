#include <thread>
#include <chrono>
#include <mutex>
#include <functional>
#include <iostream>
#include <cmath>
template <typename Res, typename... Args>
struct function_ptr_helper
{
public:
	template<typename function_type>
	static auto bind(function_type&& f) { func = std::forward<function_type>(f); }

	static auto invoke(Args... args) { return func(args...); }
	static auto* ptr() { return &invoke; }

private:
	static std::function<Res(Args ...)> func;
};

template <typename Res, typename... Args>
std::function<Res(Args ...)> function_ptr_helper<Res, Args...>::func;

template <typename Res, typename ... Args>
auto* get_fn_ptr(std::function<Res(Args...)> const& f)
{
	using type = function_ptr_helper<Res, Args...>;

	type::bind(f);
	return type::ptr();
}

struct test
{
	double operator()(double a, double b) const
	{
		return a+ b + c;
	}
	double c = 1.0;
};

void foo() {}

template <class R, class... Args>
struct CFunctionPointer
{
	std::function<R(Args...)> func;

	static R callback(void* context, Args... args)
	{
		const CFunctionPointer* unpackedThis = reinterpret_cast<const CFunctionPointer*>(context);
		return unpackedThis->func(std::forward<Args>(args)...);
	}
};

int main()
{
	std::function<void()> f1 = foo;
	auto fp = *f1.target<void(*)()>();
	auto bb = &foo;
	std::cout << "bb is " << bb << std::endl;
	std::cout << "fp is " << fp << std::endl;

	auto callable1 = CFunctionPointer<void, const int*, const int*>
	{ 
		[](const int* begin, const int* end)
		{ 
			std::cout << *begin << " " << *end <<  std::endl;
		} 
	};
	int a = 1;
	int b = 2;
	int c = 3;
	callable1.callback(&callable1, &a, &b);

	auto callable2 = CFunctionPointer<void, const int*, const int*,  const int*>
	{
		[](const int* a, const int* b,  const int* c)
		{
			std::cout << *a << " " << *b << " " << *c << std::endl;
		}
	};
	callable2.callback(&callable2, &a, &b,&c);


	//cfunc(..., &callable, &callable.callback);

	std::function<double(double, double)> f2 = test{};
	double iRet = f2(3, 4);
	typedef double(*funcPtr)(double, double);

	funcPtr fp2 = get_fn_ptr(f2);
	iRet = fp2(1,2);
	return 0;
}