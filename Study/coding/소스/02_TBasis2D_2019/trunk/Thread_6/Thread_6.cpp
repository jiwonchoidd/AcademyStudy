#include <windows.h>
#include <iostream>
#include <atomic>
#include <thread>

class Runnable
{
public :
	Runnable() : m_stop(), m_thread() {}
	virtual ~Runnable() { stop(); }

	Runnable(Runnable const&) = delete;
	Runnable& operator = (Runnable const&) = delete;

	void start()
	{ 
		m_thread = std::
		thread(&Runnable::run, this); 
	}
	void stop()
	{
		m_stop = true; 
		if (m_thread.joinable())
		{
			m_thread.join();
		}		
	}
	void suspend() { SuspendThread(m_thread.native_handle()); }
	void resume() { ResumeThread(m_thread.native_handle()); }
	bool isStop() { return m_stop; }
protected:
	virtual void run() = 0;
	std::atomic<bool> m_stop;
public:	
	std::thread m_thread;
};

class myThread : public Runnable
{
protected:
	void run()
	{
		while (!m_stop) 
		{
			std::cout << "test" << std::endl;
		}
	}
};

int main()
{
	myThread t;
	t.start();
	HANDLE threadID = t.m_thread.native_handle();
	std::this_thread::sleep_for(std::chrono::microseconds(1500));

	t.suspend();
	std::this_thread::sleep_for(std::chrono::microseconds(1500));

	t.resume();
	std::this_thread::sleep_for(std::chrono::microseconds(3500));

	t.stop();
	return 0;
}