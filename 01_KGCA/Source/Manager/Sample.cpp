#include "Sample.h"
#include <stdlib.h>
int main()
{
	Sample sample;
	bool isRun = true;
	
	if (!sample.m_FileIO.FileLoad())
	{
		sample.m_FileIO.FileLoad();
		std::cout << "\n::::::::::저장된 파일을 불러왔습니다.::::::::::\n" << std::endl;
	}
	else
	{
		std::cout << "\n::::::::::저장된 파일을 불러왔습니다.::::::::::\n" << std::endl;
		sample.m_FileIO.m_List.PrintAll();
	}
	while (isRun)
	{
		int act = 0;
		std::cout << "\n:::::::::::::활동을 선택해주세요.::::::::::::::\n\nAdd(0)  Delete(1)  Print(2)  Exit(3)" << std::endl;
		std::cin >> act;
		switch (act)
		{
		case 0: 
			#pragma region 0번은 추가
		{
			std::string name = "";
			int korScore = 0, mathScore = 0, engScore = 0;
			std::cout << "\n학생의 이름 입력하세요." << std::endl;
			std::cin >> name;
			std::cout << "\n국어 점수 입력하세요." << std::endl;
			std::cin >> korScore;
			std::cout << "\n수학 점수 입력하세요." << std::endl;
			std::cin >> mathScore;
			std::cout << "\n영어 점수 입력하세요." << std::endl;
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
			#pragma region 1번은 삭제
			system("cls");
			sample.m_FileIO.m_List.DeletePop();
			sample.m_FileIO.m_List.PrintAll();
			break;
#pragma endregion
		case 2:
			#pragma region 2번은 출력
			system("cls");
			sample.m_FileIO.m_List.PrintAll();
			break;
#pragma endregion
		case 3: 
			#pragma region 3번은 종료
			if (sample.m_FileIO.FileSave())
			{
			std::cout << "\n:::::::::::::저장완료:::::::::::::\n" << std::endl;
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