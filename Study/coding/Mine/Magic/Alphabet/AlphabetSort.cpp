#include <string>
#include <iostream>
#include <vector>

std::vector<int> solution(std::vector<int> array, std::vector<std::vector<int>> commands);

int main()
{
	std::vector<std::vector<int>> commands;
	std::vector<int> nums;
	nums.push_back(3);
	nums.push_back(6);
	nums.push_back(9);
	commands.push_back(nums);

	for (int i =0; i < commands.size(); i++)
		std::cout << commands.begin();
}
std::vector<int> solution(std::vector<int> array, std::vector<std::vector<int>> commands)
{

}