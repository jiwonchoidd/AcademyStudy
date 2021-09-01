#include <chrono>
#include <ostream>
#include <thread>
#include <iostream>
#include <cmath>
#define BASIC_SAMPLE
#ifdef  BASIC_SAMPLE
	void Test()
	{
		for (long i = 0; i < 10000000; ++i)
		{
			std::sqrt(123.456L);
		}
	}

	int main()
	{
		std::chrono::system_clock::time_point StartTime = std::chrono::system_clock::now();
		Test();
		std::chrono::system_clock::time_point EndTime = std::chrono::system_clock::now();

		std::chrono::duration<double> DefaultSec = EndTime - StartTime;
		std::chrono::nanoseconds	nano = EndTime - StartTime;
		std::chrono::microseconds	micro = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime);
		std::chrono::milliseconds	mill = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
		std::chrono::seconds		sec = std::chrono::duration_cast<std::chrono::seconds>(EndTime - StartTime);
		std::chrono::minutes		min = std::chrono::duration_cast<std::chrono::minutes>(EndTime - StartTime);
		std::chrono::hours			hour = std::chrono::duration_cast<std::chrono::hours>(EndTime - StartTime);

		std::cout << "Test() 함수를 수행하는 걸린 시간 : " << DefaultSec.count() << " default" << std::endl;
		std::cout << "Test() 함수를 수행하는 걸린 시간 : " << nano.count() << " nanoseconds" << std::endl;
		std::cout << "Test() 함수를 수행하는 걸린 시간 : " << micro.count() << " microseconds" << std::endl;
		std::cout << "Test() 함수를 수행하는 걸린 시간 : " << mill.count() << " milliseconds" << std::endl;
		std::cout << "Test() 함수를 수행하는 걸린 시간 : " << sec.count() << " seconds" << std::endl;
		std::cout << "Test() 함수를 수행하는 걸린 시간 : " << min.count() << " minutes" << std::endl;
		std::cout << "Test() 함수를 수행하는 걸린 시간 : " << hour.count() << " hour" << std::endl;
		return 0;
	}
#else
class TTimer 
{
	typedef std::chrono::high_resolution_clock high_resolution_clock;
	typedef std::chrono::milliseconds milliseconds;
public:
	explicit TTimer(bool run = false)
	{
		if (run)
			Reset();
	}

	void Reset()
	{
		_start = high_resolution_clock::now();
	}

	milliseconds Elapsed() const
	{
		return std::chrono::duration_cast<milliseconds>(high_resolution_clock::now() - _start);
	}

	template <typename T, typename Traits>
	friend std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& out, const TTimer& timer)
	{
		return out << timer.Elapsed().count();
	}
private:
	high_resolution_clock::time_point _start;
};

class TTimerThread
{
	std::thread th;
	bool running = false;

public:
	typedef std::chrono::milliseconds Interval;
	typedef std::function<void(void)> Timeout;

	void start(const Interval &interval,const Timeout &timeout)
	{
		running = true;

		th = std::thread([=]()
		{
			while (running == true) 
			{
				std::this_thread::sleep_for(interval);
				timeout();
			}
		});
		th.detach();// .join();
	}
	void stop()
	{
		running = false;
	}
};



int main()
{
	TTimer timer(true);

	// Kill some time
	for (int i = 0; i < 1000000000; i++);
	std::cout << "Elapsed time: " << std::fixed << timer << "ms\n";
	timer.Reset();

	// Kill some more time
	for (int i = 0; i < 10000000; i++);
	auto elapsed = timer.Elapsed();
	std::cout << "Elapsed time: " << std::fixed << elapsed.count() << "ms\n";

	TTimerThread tHello;
	tHello.start(std::chrono::milliseconds(100),[]() {	std::cout << "Hello!" << std::endl;	} );
	std::this_thread::sleep_for(std::chrono::seconds(2));
	tHello.stop();
}


#endif