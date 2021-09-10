#include <iostream>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <chrono>
using namespace std::chrono_literals; //100ms

//#define CV_CLASS
#ifdef CV_CLASS
std::condition_variable_any cv;
std::mutex cv_m;
int i;
void waits(int idx)
{
	std::unique_lock<std::mutex> lk(cv_m);
	if (cv.wait_for(lk, idx * 100ms, [] {return i == 1; }))
		std::cerr << "Thread " << idx << " finished waiting. i == " << i << '\n';
	else
		std::cerr << "Thread " << idx << " timed out. i == " << i << '\n';
}

void signals()
{
	std::this_thread::sleep_for(120ms);
	std::cerr << "Notifying...\n";
	cv.notify_all();

	std::this_thread::sleep_for(100ms);
	{
		std::lock_guard<std::mutex> lk(cv_m);
		i = 1;
	}
	std::cerr << "Notifying again...\n";
	cv.notify_all();
}

int main()
{
	std::thread t1(waits, 1), t2(waits, 2), t3(waits, 3), t4(signals);
	t1.join(); t2.join(), t3.join(), t4.join();
}
#else

struct ProcessData 
{
	int							m_iValue=0;
	std::recursive_mutex		m_hMutex;
	std::condition_variable_any m_hCva;
	bool						m_bDataReady = false;
public:
	void prepare_data_for_processing()
	{
		while(1)
		{
			{
				std::lock_guard<std::recursive_mutex> lk(m_hMutex);
				m_bDataReady = true;
			}
			std::this_thread::sleep_for(1s);
		}
		m_hCva.notify_one();
	}

	void wait_for_data_to_process()
	{
		while (1)
		{
			{
				std::unique_lock<std::recursive_mutex> lk(m_hMutex);
				m_hCva.wait(lk, [this] { return m_bDataReady; });
				process_data(m_iValue++);
			}
			std::this_thread::sleep_for(1s);
		}
	}

private:
	void process_data(int iValue)
	{
		std::cout << "data=" << iValue <<std::endl;
	}
};

int main()
{
	ProcessData p;

	std::thread t1([&] { p.prepare_data_for_processing(); });
	std::thread t2([&] { p.wait_for_data_to_process(); });

	t1.join();
	t2.join();
}
#endif