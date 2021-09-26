#include <iostream>
#include <algorithm>
//https://www.acmicpc.net/problem/1145

int main()
{
	int input[5];
	for (int i = 0; i < 5; i++)
	{
		std::cin >> input[i];
	}

	int array1[10];
	int array2[10];
	int temp1 = 0;
	int temp2 = 0;
	int same = 0;
	for (int i = 0; i < 4; i++)
	{
		temp1 = input[i];
		temp2 = input[i + 1];
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				array1[j] = temp1 * j;
				array2[j] = temp2 * j;
			}
			for (int j = 0; j < 6; j++)
			{
				if (array1[j] == array2[i])
				{
					same = array1[j];
					break;
				}
			}
		}
	}

	
	std::cout << same;
	
}