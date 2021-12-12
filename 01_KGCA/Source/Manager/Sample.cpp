#include "Sample.h"
#include <stdlib.h>
int main()
{
	Sample sample;
	bool isRun = true;
	
	if (!sample.m_FileIO.FileLoad())
	{
		sample.m_FileIO.FileLoad();
		std::cout << "\n::::::::::����� ������ �ҷ��Խ��ϴ�.::::::::::\n" << std::endl;
	}
	else
	{
		std::cout << "\n::::::::::����� ������ �ҷ��Խ��ϴ�.::::::::::\n" << std::endl;
		sample.m_FileIO.m_List.PrintAll(); 
	}
	while (isRun)
	{
		int act = 0;
		std::cout << "\n:::::::::::::Ȱ���� �������ּ���.::::::::::::::\n\nAdd(0)  Delete(1)  Print(2)  Save&Exit(3)" << std::endl;
		std::cin >> act;
		switch (act)
		{
		case 0: 
			#pragma region 0���� �߰�
		{
			std::string name = "";
			int korScore = 0, mathScore = 0, engScore = 0;
			std::cout << "\n�л��� �̸� �Է��ϼ���." << std::endl;
			std::cin >> name;
			if (name.length() < 2)
			{
				system("cls");
				std::cout << "\n�߸��� �̸��Դϴ�." << std::endl;
				continue;
			}
			std::cout << "\n���� ���� �Է��ϼ���." << std::endl;
			std::cin >> korScore;
			(korScore > 100) ? korScore = 100 : korScore;
			std::cout << "\n���� ���� �Է��ϼ���." << std::endl;
			std::cin >> mathScore;
			(mathScore > 100) ? mathScore = 100 : mathScore;
			std::cout << "\n���� ���� �Է��ϼ���." << std::endl;
			std::cin >> engScore;
			(engScore > 100) ? engScore = 100 : engScore;
			system("cls");
			KStudent* a = new KStudent(name, korScore, mathScore, engScore);
			//��ü ��ü�� ���ø��� �̿��� �ְ� ������.
			sample.m_FileIO.m_List.AddLink(a);
			sample.m_FileIO.FileSave();
			sample.m_FileIO.m_List.PrintAll();
		break;
		}
#pragma endregion
		case 1:
			#pragma region 1���� ����
			system("cls");
			sample.m_FileIO.m_List.DeletePop();
			sample.m_FileIO.m_List.PrintAll();
			break;
#pragma endregion
		case 2:
			#pragma region 2���� ���
			system("cls");
			sample.m_FileIO.m_List.PrintAll();
			break;
#pragma endregion
		case 3: 
			#pragma region 3���� ����
			if (sample.m_FileIO.FileSave())
			{
			std::cout << "\n:::::::::::::����Ϸ�:::::::::::::\n" << std::endl;
			}
			isRun = false;
			break;
#pragma endregion
		default: 
			if (sample.m_FileIO.FileSave())
			{
				std::cout << "\n:::::::::::::����Ϸ�:::::::::::::\n" << std::endl;
			}
			isRun = false;
			break;
		}
	}
}