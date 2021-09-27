#include <iostream>
#include <vector>
int main()
{
	double input = 0;
	double max = 0;
	double avg = 0;
	std::vector<double> score;
	
	std::cin >> input;

	for (int i = 0; i < input; i++)
	{
		int scoreinput = 0;
		std::cin >> scoreinput;
		score.push_back(scoreinput);
	}

	//최댓값 구하기
	for (auto iter = score.begin(); iter < score.end(); iter++)
	{
		if (max < *iter)
		{
			max = *iter;
		}
	}

	for (auto iter = score.begin(); iter < score.end(); iter++)
	{
		avg = avg + (*iter) / max * 100;
	}

	std::cout << avg / input;
}