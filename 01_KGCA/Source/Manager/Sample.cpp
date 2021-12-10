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
		std::cout << "\n:::::::::::::Ȱ���� �������ּ���.::::::::::::::\n\nAdd(0)  Delete(1)  Print(2)  Exit(3)" << std::endl;
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
			std::cout << "\n���� ���� �Է��ϼ���." << std::endl;
			std::cin >> korScore;
			std::cout << "\n���� ���� �Է��ϼ���." << std::endl;
			std::cin >> mathScore;
			std::cout << "\n���� ���� �Է��ϼ���." << std::endl;
			std::cin >> engScore;
			system("cls");
			KStudent* a = new KStudent(name, korScore, mathScore, engScore);
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
			isRun = false;
			break;
		}
	}
}