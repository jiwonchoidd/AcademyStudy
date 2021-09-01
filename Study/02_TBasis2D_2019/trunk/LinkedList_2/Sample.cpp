// 헤드 노드부터 데이터 저장
// 샘플 데이터 생성 시  오름차순 및 내림차순 정렬하여 저장
#include "Sample.h"
void  Initialize()
{
	SORTFUNCTION = Ascending;
	srand(time(NULL));
}
void main()
{
	Initialize();
	FileLoad();

	int iSelect = 0;
	while (1)
	{
		MenuPrint();
		scanf("%d", &iSelect);
		if (iSelect == EXIT) break;
		system("cls");
		// 작업리스트
		switch (iSelect)
		{
		case SCREEN_PRINT:
		{
			if (ScreenPrint() > 0)
				printf("\n정상적으로 출력.");
			else
				printf("\n데이터가 존재하지 않습니다.");
		}break;
		case NEW_DATA:
		{
			NewData();
		}break;
		case FILE_SAVE:
		{
			if (FileSave() > 0)
				printf("\n정상적으로 저장.");
			else
				printf("\n문제가 있어 저장 하지 못했습니다.");
		}break;
		case FILE_LOAD:
		{
			if (FileLoad() > 0)
				printf("\n정상적으로 로드됨.");
			else
				printf("\n로드 실폐1.");
		}break;
		case DATA_UPDATE:
		{
			while (UpdateData() < 0)
			{
				printf("검색 실폐!");
			}
		}break;
		case CREATE_SAMPLE_DATA:
		{
			CreateSampleData(Ascending); // 오름차순
			//CreateSampleData(Descending); // 내림차순
		}break;
		}
	}

	AllDelLink();
	printf("\n아무키나 누르시면 종료됩니다.");
	_getch();
}