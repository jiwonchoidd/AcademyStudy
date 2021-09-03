// Thread_4.cpp : 클래스 메소드 사용(특정 인스턴스의 메소드를 별도 Thread로 실행시킬 수 있는 장점)
//

#include "pch.h"
#include <cstdio>
#include <thread>

using namespace std;

class Counter 
{
public:
	Counter(int id, int length) 
	{
		mId = id;
		mLength = length;
	}

	void loop() const 
	{
		for (int i = 1; i <= mLength; i++) 
		{
			printf("counter[%d] : %d\n", mId, i);
		}
	}

private:
	int mId;
	int mLength;
};

int main() 
{
	Counter c1(1, 7);

	thread t1{ &Counter::loop, &c1 };

	t1.join();

	return 0;
}