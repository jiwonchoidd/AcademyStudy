#include <iostream>
#include <future>//std::async
const int number = 123456;

void for_print(char c) 
{
	for (int i = 0; i < 3; i++)
	{
		printf("%c번 Thread : %d\n", c, i);
	}
}
// 소수계산
bool IsPrimeNumber(int x) 
{
	for (int i = 2; i < x; ++i)
	{
		if (x % i == 0)
		{
			return false;
		}
	}
	return true;
}
template <typename it>
int parallel_sum(it start, it end) 
{
	int len = end - start;
	if (len <= 1)
		return 0;

	it newStart = start + 200;
	auto hendle = std::async(parallel_sum<it>, newStart, end);
	int sum = 0;
	for_each(start, newStart, [&](int n) { sum += n; /* std::cout << n << "\n"; */ });
	return sum + hendle.get();
}
int main() 
{
	{
		std::future<void> a = std::async(std::launch::async, for_print, 'a');
		std::future<void> b = std::async(std::launch::deferred, for_print, 'b');
		std::future<void> c = std::async(for_print, 'c');
		b.get();
	}

	{
		auto c = std::async(for_print, 'd');
		std::future<int> d;

		try {
			c.get();
			d.get();
		}
		catch (const std::future_error& e) {
			std::cout << "Caught a future_error with code \"" << e.code()
				<< "\"\nMessage: \"" << e.what() << "\"\n";
		}
	}

	{
		// call function asynchronously:
		std::future<bool> fut = std::async(IsPrimeNumber, number);

		// do something while waiting for function to set future:
		std::cout << "checking, please wait";
		std::chrono::milliseconds span(100);
		//std::chrono::duration<int> span (1);

		while (fut.wait_for(span) != std::future_status::ready) 
		{  // == std::future_status::timeout
			std::cout << '.';
			std::cout.flush();
		}

		bool x = fut.get();     // retrieve return value

		std::cout << "\n" << number << " " << (x ? "is" : "is not") << " prime.\n";
	}

	{
		std::vector<int> v;
		v.reserve(1001);
		for (int i = 1; i < 1001; i++) v.emplace_back(i);
		std::cout << "sum : " << parallel_sum(v.begin(), v.end()) << "\n";
	}
	return 0;
}