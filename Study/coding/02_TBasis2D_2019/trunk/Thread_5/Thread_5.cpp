#include <windows.h>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <queue>
#include <chrono>
#include <string>
#include <list>
//using namespace std;//3000ms
//using namespace std::chrono_literals; //3000ms
int g_iSum = 0;
std::mutex mtx;
#define tSleep(value) std::this_thread::sleep_for(value);
// 0+1, 1+2, 3+4    7+8    15+16 31+32
//  1,   3,    7    15      31    63 
void counter(int id, int size)
{
	using namespace std;
	int iCount = 0;
	for (int i = 0; i < size; i++)
	{
		mtx.lock();
		iCount = g_iSum;
		std::cout << ++iCount << std::endl;
		g_iSum += iCount;
		mtx.unlock();
		//tSleep(1c);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		//std::this_thread::sleep_for(//std::chrono::duration<int>(3));
		std::this_thread::sleep_for(3000ms);
		std::this_thread::sleep_for(3s);
	}
}
class Counter
{
public:
	int   m_iId;
	int	  m_iSize;
	Counter(int id, int size)
	{
		m_iId = id;
		m_iSize = size;
	}
	void loop()
	{
		for (int i = 0; i < m_iSize; i++)
		{
			std::cout << m_iId << std::endl;
		}
	}
};
class TThread
{
public:
	std::thread			 m_thread;
	std::atomic<bool>    m_stop;
public:
	void start()
	{
		m_thread = std::thread(&TThread::run, this);
	};
	void stop()
	{
		m_stop = true;
		m_thread.join();
	};
	void suspend()
	{
		SuspendThread(m_thread.native_handle());
	}
	void resume()
	{
		ResumeThread(m_thread.native_handle());
	}
protected:
	virtual void run() = 0;
public:
	TThread() : m_stop(), m_thread() {}
	virtual ~TThread()
	{
	}
};
class TSubThread : public TThread
{
public:
	int k = 0;
	void run()
	{
		while (!m_stop)
		{
			std::cout << "1";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
};
void work()
{
	std::thread tt([]()
	{
		std::cout << "thread-start" << std::endl;
		// SendAllCharacter()
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "thread-end" << std::endl;
	});
	tt.detach();
}
void main()
{
	std::cout.sync_with_stdio(true);
	std::thread tt([]()
	{
		std::cout << "thread" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
	});
	tt.join();
	//tt.detach();

	for (int i = 0; i < 10; i++)
	{
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		work();
	}

	TSubThread t;
	t.start();
	//t.m_thread.detach();

	std::this_thread::sleep_for(std::chrono::seconds(3));
	t.suspend();
	std::cout << "suspend" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));

	t.resume();
	std::cout << "resume" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	t.stop();

	std::thread t1(counter, 1, 3);
	std::thread t2{ counter,  1, 3 };
	t1.join();//대기함수
	t2.join();//대기함수	
	std::cout << g_iSum << std::endl;

	auto Lambda = [](int id, int size)->void
	{
		for (int i = 0; i < size; i++)
		{
			std::cout << id << std::endl;
		}
	};
	std::thread t3(Lambda, 88, 3);
	t3.join();

	Counter c1(77, 3);
	std::thread t4(&Counter::loop, &c1);
	t4.join();
}