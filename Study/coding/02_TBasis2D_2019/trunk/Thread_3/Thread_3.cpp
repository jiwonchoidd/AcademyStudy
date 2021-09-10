#include <cstdio>
#include <thread>

using namespace std;

int main() 
{
	thread t1([](int id, int length) 
	{
		for (int i = 1; i <= length; i++) 
		{
			printf("counter[%d] : %d\n", id, i);
		}
	}, 1, 7);

	t1.join();

	return 0;
}