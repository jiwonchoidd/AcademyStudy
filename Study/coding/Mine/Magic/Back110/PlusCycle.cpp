#include <iostream>
//https://www.acmicpc.net/problem/1110
//0~99 ���� 10���� ������ �տ� 0�� ���̰� ���ڶ��� ���ڸ� ����

int main()
{
	int		cycle = 0;
	int		input = 0;
	int		match = 0;
	int		ten = 0;
	int		one = 0;
	std::cin >> input;
	//�Է°��� ��ĩ���� ���������� �ݺ�
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