#include <iostream>
#include <list>
class ListStl
{
public :
    std::list<int> list;
    int insert = 0;
	//몇개 넣을것인지?
	int input = 0;
};

int main()
{
	ListStl st;
	std::cin >> st.input;
	for (int i = 0; i < st.input; i++) {
		std::cin >> st.insert;
		st.list.insert(st.list.begin(),st.insert);
	}
	st.list.sort();
	
	for (std::list<int>::iterator iter = st.list.begin();
		iter != st.list.end();
		iter++)
	{
		// 인덱싱 불가
		std::cout << *iter << "\n";
	}
}
