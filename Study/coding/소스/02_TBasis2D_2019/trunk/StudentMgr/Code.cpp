//(비연결리스트 사용버전)
#include "Code.h"
int main()
{
	// 최근 저장된 파일로 부터 데이터를 로드한다.
	// 저장 파일이 없으면 g_iUseSampleData값에 따라서 샘플데이터가 만들어 진다.
	FileLoad();

	srand(time(NULL));
	int select;
	while (1)
	{
		printf("\n저장:0,로드:1,신규:2,출력:3,검색번호:4,검색이름:5,초기:9,종료(99)");
		scanf("%d", &select);
		if (select == Exit) break;
		system("cls");
		switch (select)
		{
			case DataSave: {
				if (FileSave() > 0)
					printf("\n파일이 저장되었습니다.");
				else
					printf("\n파일을 저장하지 못했습니다.");
			}break;
			case DataLoad: {
				if (FileLoad() > 0)
					printf("\n파일이 로드되었습니다.");
				else
					printf("\n파일을 로드하지 못했습니다.");
			}break;
			case DataNew: {
				NewData();
			}break;
			case DataPrint: {
				PrintData();
			}break;			
			case DataFindIndex: {
				FindIndex();
			}break;
			case DataFindName: {
				FindName();
			}break;			
			case SampleData: {
				// 모든 g_pStudentData가 삭제되고 샘플용 데이터가 생성된다.
				CreateSampleData();
			}break;
		}
	}
	free(g_pStudentData);

	printf("\n아무키나 누르시면 종료됩니다.!");
	_getch();
	return 0;
}

