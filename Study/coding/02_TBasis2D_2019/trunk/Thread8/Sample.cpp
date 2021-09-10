#include <iostream>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <functional>

struct TData 
{
	explicit TData(int num) : m_iValue{ num } {}
	int			m_iValue;
	std::mutex	m_hMutex;
};

void Worker(TData &d1, TData &d2, int iValue) 
{
	// std::unique_lock은 기본적으로 생성과 동시에 lock이 걸리고 소멸시에 unlock된다.
	// 하지만 객체를 생성해 두고 나중에 필요한 시점에 락 점유를 시도할 수 있다.
	std::unique_lock<std::mutex> lock1(d1.m_hMutex, std::defer_lock);
	std::unique_lock<std::mutex> lock2(d2.m_hMutex, std::defer_lock);

	// 2개의 뮤텍스를 동시에 lock
	std::lock(lock1, lock2);

	d1.m_iValue -= iValue;
	d2.m_iValue += iValue;
}

// std::condition_variable은 다른 스레드가 공유변수 변경하고 통지할 때가지
// 스레드들이 대기하도록 하는 동기화 방법이다.
// 뮤텍스와 연동되어야 안전하게 사용된다.
std::condition_variable	g_hEvent;
std::mutex				g_hMutex;
bool					g_bDataReady = false;

void WaitEventThread()
{
	::std::thread([]
	{
		for (;;)
		{
			::std::unique_lock<decltype(g_hMutex)> hLock(g_hMutex);
			// 자동적으로 hLock.unlock()가 호출되고 스레드가 블록되어 알림신호를 대기한다.
			g_hEvent.wait(hLock, [] { return g_bDataReady; });
			std::cout << "t5 ";// << std::endl;
			g_bDataReady = false;
		}
	}
	).detach();
}

int main()
{
	std::cout.sync_with_stdio(true);
	///
	{
		std::mutex	m1, m2;
		std::thread t1([&]() 
		{
			std::lock(m1, m2);
			std::cout << "t1 ";// << std::endl;
			m1.unlock();
			m2.unlock();
		});
		std::thread t2([&]() 
		{
			std::lock(m1, m2);
			std::cout << "t2 ";// << std::endl;
			m1.unlock();
			m2.unlock();
		});
		t1.join();
		t2.join();
	
		////
		std::thread t3([&]() 
		{
			std::lock_guard<std::mutex> lock_guard(m1);
			for (int i = 0; i < 100; i++) 
			{
				std::cout << "t3 ";// << std::endl;
			}
		});
		std::thread t4([&]() 
		{
			std::lock_guard<std::mutex> lock_guard(m1);
			for (int i = 0; i < 100; i++) 
			{
				std::cout << "t4 ";//<< std::endl;
			}
		});
		t3.join();
		t4.join();
	}

	{
		TData d1(100);
		TData d2(50);

		std::thread t3(Worker, std::ref(d1), std::ref(d2), 10);
		std::thread t4(Worker, std::ref(d2), std::ref(d1), 5);

		t3.join();
		t4.join();
	}

	// 
	{
		WaitEventThread();

		auto lambda = []()
		{
			for (;;)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
				{
					::std::unique_lock<std::mutex> hLock(g_hMutex);// , std::defer_lock);			
					//g_hMutex.lock();
					g_bDataReady = true;
					g_hEvent.notify_one();// 대기중인 1개의 스레드에 신호를 준다.
					//g_hMutex.unlock();							
				}
			}
		};
		std::thread t5(lambda);
		t5.join();
	}
}