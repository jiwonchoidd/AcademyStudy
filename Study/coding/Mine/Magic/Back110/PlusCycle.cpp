#include <iostream>
//https://www.acmicpc.net/problem/1110
//0~99 정수 10보다 작으면 앞에 0을 붙이고 각자라의 숫자를 더함

int main()
{
	int		cycle = 0;
	int		input = 0;
	int		match = 0;
	int		ten = 0;
	int		one = 0;
	std::cin >> input;
	//입력값과 매칫값이 같을때까지 반복
	while (input != match && input!=0)
	{
		if (match != 0)
		{
			ten = (int)match / 10;
			one = (int)match % 10;
		}
		else
		{
			ten = (int)input / 10;
			one = (int)input % 10;
		}

		match = ten + one;
		match = (match%10) + (one * 10);
		cycle++;
	}
	if (cycle == 0)
	{
		cycle = 1;
	}
	std::cout << cycle << std::endl;
}