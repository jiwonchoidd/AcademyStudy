#include "Sample.h"
#include <stdlib.h>
int main()
{
	Sample sample;
	bool isRun = true;
	while (isRun)
	{
		int act = 0;
		std::cout << " Ȱ���� �������ּ���. 0 : �߰�, 1 : ����, 2 : ���, 3 : ����" << std::endl;
		std::cin >> act;
		switch (act)
		{
		case 0: {
			int input = 0;
			std::cout << "���� �Է��ϼ���" << std::endl;
			std::cin >> input;
			system("cls");
			Person* a= new Person;
			a->m_Value = input;
			sample.m_LL.AddLink(a);
			sample.m_LL.PrintAll();
		}break;
		case 1: {
			system("cls");
			sample.m_LL.DeletePop();
			sample.m_LL.PrintAll();
		}break;
		case 2: {
			system("cls");
			sample.m_LL.PrintAll();
		}break;
		case 3: {
			isRun=false;
		}break;
		default: act = true;
			break;
		}
	}
}