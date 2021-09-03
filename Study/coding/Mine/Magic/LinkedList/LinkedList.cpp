#include "TLinkedList.h"
#include <cstdlib>
#include <ctime>
void Activity(int ran);
int total=0;
int random=0;
int main()
{
	TLinkedList t;
	std::cout << "사람의 수 1~100 입력하세요 : ";
	std::cin >> total;
	srand((unsigned int)time(NULL));
	random = rand() % total + 1;
	for (int i = 1; i <= total; i++)
	{
		TNode* newNode = new TNode;
		newNode->m_iValue = i;
		t.AddBackwardLink(newNode);
	}
	t.Show();
	std::cout << " 랜덤숫자 : " << random << "\n";
	bool isRun = false;
	while (!isRun)
	{
		int act = 0;
		std::cin >> act;
		switch (act)
		{
		case 0: {
			system("cls");
			TNode* Temp= t.Search(random);
			std::cout <<random<<"번째 자리 "<< Temp->m_iValue << "을 삭제\n";
			t.Killed(Temp, random);
			t.Show();
		}break;
		default: act = true;
			break;
		}
	}
}

