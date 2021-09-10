#include <iostream>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <functional>

// 속성->C/C++언어->C++언어표준->ISO C++17 표준(/std:c++17)  선택해야 한다.
using namespace std;
once_flag f;
mutex cout_sync;
void my_pause()
{
	volatile int x = 0;
	for (int i = 0; i < 2'000'000'000; ++i) { x++; }
}
void thr(int id)
{
	auto start = chrono::system_clock::now();
	call_once(f, my_pause);
	auto end = chrono::system_clock::now();
	std::scoped_lock l { cout_sync };

	//static std::mutex sMutex;
	//std::scoped_lock<std::mutex > lock(sMutex);
	cout << "Thread " << id << " finished in " << 
		(static_cast<chrono::duration<double>>(end - start)).count() << " sec" << endl;
}
int main()
{
	vector<thread> threads;
	for (int i = 0; i < 4; i++)
	{
		threads.emplace_back(thr, i);
		threads.back().detach();
	}
	this_thread::sleep_for(chrono::seconds(5));
	/*for (int i = 0; i < 4; i++)
	{
		threads.emplace_back(thr, i);
		threads.back().join();
	}*/
	return 0;
}