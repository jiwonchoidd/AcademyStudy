#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
using namespace std;
int iCount = 0;
//std::atomic<int> iCount = 0;

std::mutex mt;
//void func(int& iCnt)
void funcAdd()
{
	cout << std::this_thread::get_id() << endl;
	for (int i = 0; i < 100; ++i)
	{	
		mt.lock();			
		// 1�� ���� sleep�Ѵ�.
		std::this_thread::sleep_for(std::chrono::duration<int, milli>(1000));
		// c++14 ���������� ����ó�� �����ϰ� ó���� �� �ִ�.
		//std::this_thread::sleep_for(1s);
			++iCount;
			cout << std::this_thread::get_id() << " " << iCount << endl;
		mt.unlock();
	}
}

void main()
{	
	//thread th1(func, std::ref(iCount));
	//thread th2(func, std::ref(iCount));
	thread th1(funcAdd);
	thread th2(funcAdd);
	th1.join();
	th2.join();
	cout << iCount << endl;
}