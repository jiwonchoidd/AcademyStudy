#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <memory>
#include <algorithm>
//sample
#include <iostream>
#include <string>

void TimerCall(int i)
{
	std::cout << i << std::endl;
}
template<class T>
class TimerThread
{
	typedef std::chrono::high_resolution_clock clock_t;

	struct TimerInfo
	{
		clock_t::time_point m_TimePoint;
		T m_User;

		template <class TArg1>
		TimerInfo(clock_t::time_point tp, TArg1 && arg1)
			: m_TimePoint(tp)
			, m_User(std::forward<TArg1>(arg1))
		{
		}

		template <class TArg1, class TArg2>
		TimerInfo(clock_t::time_point tp, TArg1 && arg1, TArg2 && arg2)
			: m_TimePoint(tp)
			, m_User(std::forward<TArg1>(arg1), std::forward<TArg2>(arg2))
		{
		}
	};

	std::unique_ptr<std::thread> m_Thread;
	std::vector<TimerInfo>       m_Timers;
	std::mutex                   m_Mutex;
	std::condition_variable      m_Condition;
	bool                         m_Sort;
	bool                         m_Stop;

	void TimerLoop()
	{
		for (;;)
		{
			std::unique_lock<std::mutex>  lock(m_Mutex);

			while (!m_Stop && m_Timers.empty())
			{
				m_Condition.wait(lock);
			}

			if (m_Stop)
			{
				return;
			}

			if (m_Sort)
			{
				//Sort could be done at insert
				//but probabily this thread has time to do
				std::sort(m_Timers.begin(),
					m_Timers.end(),
					[](const TimerInfo & ti1, const TimerInfo & ti2)
				{
					return ti1.m_TimePoint > ti2.m_TimePoint;
				});
				m_Sort = false;
			}

			auto now = clock_t::now();
			auto expire = m_Timers.back().m_TimePoint;

			if (expire > now) //can I take a nap?
			{
				auto napTime = expire - now;
				m_Condition.wait_for(lock, napTime);

				//check again
				auto expire = m_Timers.back().m_TimePoint;
				auto now = clock_t::now();

				if (expire <= now)
				{
					TimerCall(m_Timers.back().m_User);
					m_Timers.pop_back();
				}
			}
			else
			{
				TimerCall(m_Timers.back().m_User);
				m_Timers.pop_back();
			}
		}
	}

	template<class T, class TArg1>
	friend void CreateTimer(TimerThread<T>& timerThread, int ms, TArg1 && arg1);

	template<class T, class TArg1, class TArg2>
	friend void CreateTimer(TimerThread<T>& timerThread, int ms, TArg1 && arg1, TArg2 && arg2);

public:
	TimerThread() : m_Stop(false), m_Sort(false)
	{
		m_Thread.reset(new std::thread(std::bind(&TimerThread::TimerLoop, this)));
	}

	~TimerThread()
	{
		m_Stop = true;
		m_Condition.notify_all();
		m_Thread->join();
	}
};

template<class T, class TArg1>
void CreateTimer(TimerThread<T>& timerThread, int ms, TArg1 && arg1)
{
	{
		std::unique_lock<std::mutex> lock(timerThread.m_Mutex);
		timerThread.m_Timers.emplace_back(TimerThread<T>::TimerInfo(TimerThread<T>::clock_t::now() + std::chrono::milliseconds(ms),
			std::forward<TArg1>(arg1)));
		timerThread.m_Sort = true;
	}
	// wake up
	timerThread.m_Condition.notify_one();
}

template<class T, class TArg1, class TArg2>
void CreateTimer(TimerThread<T>& timerThread, int ms, TArg1 && arg1, TArg2 && arg2)
{
	{
		std::unique_lock<std::mutex> lock(timerThread.m_Mutex);
		timerThread.m_Timers.emplace_back(TimerThread<T>::TimerInfo(TimerThread<T>::clock_t::now() + std::chrono::milliseconds(ms),
			std::forward<TArg1>(arg1),
			std::forward<TArg2>(arg2)));
		timerThread.m_Sort = true;
	}
	// wake up
	timerThread.m_Condition.notify_one();
}



int main()
{
	std::cout << "start" << std::endl;
	TimerThread<int> timers;

	CreateTimer(timers, 1000, 100);
	CreateTimer(timers, 2000, 200);
	CreateTimer(timers, 3000, 300);

	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::cout << "end" << std::endl;
}