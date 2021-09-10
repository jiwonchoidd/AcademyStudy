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
	// std::unique_lock�� �⺻������ ������ ���ÿ� lock�� �ɸ��� �Ҹ�ÿ� unlock�ȴ�.
	// ������ ��ü�� ������ �ΰ� ���߿� �ʿ��� ������ �� ������ �õ��� �� �ִ�.
	std::unique_lock<std::mutex> lock1(d1.m_hMutex, std::defer_lock);
	std::unique_lock<std::mutex> lock2(d2.m_hMutex, std::defer_lock);

	// 2���� ���ؽ��� ���ÿ� lock
	std::lock(lock1, lock2);

	d1.m_iValue -= iValue;
	d2.m_iValue += iValue;
}

// std::condition_variable�� �ٸ� �����尡 �������� �����ϰ� ������ ������
// ��������� ����ϵ��� �ϴ� ����ȭ ����̴�.
// ���ؽ��� �����Ǿ�� �����ϰ� ���ȴ�.
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
			// �ڵ������� hLock.unlock()�� ȣ��ǰ� �����尡 ��ϵǾ� �˸���ȣ�� ����Ѵ�.
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
					g_hEvent.notify_one();// ������� 1���� �����忡 ��ȣ�� �ش�.
					//g_hMutex.unlock();							
				}
			}
		};
		std::thread t5(lambda);
		t5.join();
	}
}