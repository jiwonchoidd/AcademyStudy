#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
int main()
{
    // 벡터는 가변 배열이다. list[5]
    std::vector<int> list;
    std::stack<int> stack;
    std::queue<int> queue;
    list.resize(5);
    for (int i = 0; i < 5; i++)
    {
        list[i] = rand() % 100;
    }
    //std::sort(list.begin(), list.end());

    std::vector<int> list2;
    for (int i = 0; i < 5; i++)
    {
        list2.push_back(i * 10);
    }
    std::vector<int> copy;
    std::copy(list2.begin(), list2.end(),
        std::back_inserter(list));
    std::copy(list2.begin(), list2.end(),
        list.begin());

    std::vector<int> merge;
    std::merge(list2.begin(), list2.end(),
        list.begin(), list.end(),
        std::back_inserter(merge));
    for (int i = 0; i < list.size(); i++)
    {
        std::cout << list[i] << "\n";
    }
    /*  for (std::vector<int>::iterator iter=list.begin();
           iter != list.end();
           iter++)
      {
          int iValue = *iter;
          std::cout << iValue << "\n";
      }*/
    std::cout << "Hello World!\n";
}