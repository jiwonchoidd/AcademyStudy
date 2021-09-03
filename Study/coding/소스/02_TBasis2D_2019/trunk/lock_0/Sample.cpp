#include <mutex>
#include <vector>
#include <queue>
#include <thread>
#include <iostream>
#include <functional>
#include <chrono>

static std::mutex				sQueueLock;
static std::condition_variable	sEvent;
static std::queue<int>			sQueue;

void Consume()
{
	while (true)
	{
		int val;
		{
			std::unique_lock<std::mutex> lock(sQueueLock);
			sEvent.wait(lock, [] {return !sQueue.empty(); });

			val = sQueue.front();
			sQueue.pop();
		}

		std::cout << val << std::endl;
	}
}

void Produce()
{
	std::unique_lock<std::mutex> lock(sQueueLock);
	sQueue.push(1);

	sEvent.notify_one();
}


int main()
{
	{
		std::thread producer(Produce);
		std::thread consumer(Consume);

		consumer.join();
		producer.join();
	}
	{
		int			foo_count = 0;
		std::mutex	foo_count_mutex;

		int			bar_count = 0;
		std::mutex	bar_count_mutex;

		bool		done = false;
		std::mutex	done_mutex;

		int			overall_count = 0;

		auto increment = [](int &counter, std::mutex &m, const char *desc)
		{
			for (int i = 0; i < 10; ++i)
			{
				std::unique_lock<std::mutex> lock(m);
				++counter;
				std::cout << desc << ": " << counter << '\n';
				lock.unlock(); // 명시적 unlock 생략가능함.
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		};

		std::thread increment_foo(increment, std::ref(foo_count), std::ref(foo_count_mutex), "foo");
		std::thread increment_bar(increment, std::ref(bar_count), std::ref(bar_count_mutex), "bar");

		std::thread update_overall([&]()
		{
			done_mutex.lock();
			while (!done)
			{
				done_mutex.unlock();
				// 성공 = -1, 실폐 인덱스 0~ 반환
				int result = std::try_lock(foo_count_mutex, bar_count_mutex);
				if (result == -1)
				{
					overall_count += foo_count + bar_count;
					foo_count = 0;
					bar_count = 0;
					std::cout << "overall: " << overall_count << '\n';
					foo_count_mutex.unlock();
					bar_count_mutex.unlock();
				}
				std::this_thread::sleep_for(std::chrono::seconds(2));
				done_mutex.lock();
			}
			done_mutex.unlock();
		});

		increment_foo.join();
		increment_bar.join();
		done_mutex.lock();
		done = true;
		done_mutex.unlock();
		update_overall.join();

		std::cout << "Done processing\n"
			<< "foo: " << foo_count << '\n'
			<< "bar: " << bar_count << '\n'
			<< "overall: " << overall_count << '\n';
	}
}