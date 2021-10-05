#include<iostream>
#include<string>
#include <algorithm>
#include <map>
int main()
{
	char what = '?';
	int max = 0;
	int min = 65;
	std::string word;
	std::cin >> word;
	//다 대문자로 만들어주자
	for (int i = 0; i < word.length(); i++)
	{
		if (word[i] >= 97)
		{
			word[i] = (char)word[i] - 32;
		}
	}
	for (int i = 0; i <= word.length(); i++)
	{
		if (word[i] < min)
		{
			min = word[i];

		}
		if (word[i] > max)
		{
			max = word[i];
			
		}
		else if (min == max)
		{
			max= '?';
		}
	}

	std::cout << (char)max;
}