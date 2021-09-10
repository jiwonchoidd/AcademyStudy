#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <queue>
#include <chrono>
#include <string>

typedef std::recursive_mutex g_rMutex;

int main()
{
	std::queue<std::string> szQueue;
	g_rMutex rMutex;
	std::condition_variable_any cva;
	bool done = false;
	bool notified = false;

	std::thread producer([&] {
		std::string s;
		do {
			std::getline(std::cin, s);
			std::lock_guard<g_rMutex> lock(rMutex);
			szQueue.push(s);
			notified = true;
			cva.notify_one();
		} while (s.compare("exit"));

		std::lock_guard<g_rMutex> lock(rMutex);
		done = true;
		cva.notify_all();
	});

	std::vector<std::thread> threadlist;
	for (int i = 0; i < 5; ++i)
	{
		threadlist.push_back(std::thread([&, i] 
		{
			while (!done)
			{
				{
					std::unique_lock<g_rMutex> lock(rMutex);
					cva.wait(lock);
					while (!done && !szQueue.empty())
					{
						std::cout << "consumeing " << szQueue.front() << " by consumer #" << i << "\n";
						szQueue.pop();
					}
				}
				std::this_thread::sleep_for(std::chrono::seconds(1)); 
			}
		}));
	}

	producer.join();

	for (int i = 0; i < 5; ++i)
	{
		threadlist[i].detach(); 
	}
}

