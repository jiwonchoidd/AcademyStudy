// ��� ������ ������ ����
// ���� ������ ���� ��  �������� �� �������� �����Ͽ� ����
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
		// �۾�����Ʈ
		switch (iSelect)
		{
		case SCREEN_PRINT:
		{
			if (ScreenPrint() > 0)
				printf("\n���������� ���.");
			else
				printf("\n�����Ͱ� �������� �ʽ��ϴ�.");
		}break;
		case NEW_DATA:
		{
			NewData();
		}break;
		case FILE_SAVE:
		{
			if (FileSave() > 0)
				printf("\n���������� ����.");
			else
				printf("\n������ �־� ���� ���� ���߽��ϴ�.");
		}break;
		case FILE_LOAD:
		{
			if (FileLoad() > 0)
				printf("\n���������� �ε��.");
			else
				printf("\n�ε� ����1.");
		}break;
		case DATA_UPDATE:
		{
			while (UpdateData() < 0)
			{
				printf("�˻� ����!");
			}
		}break;
		case CREATE_SAMPLE_DATA:
		{
			CreateSampleData(Ascending); // ��������
			//CreateSampleData(Descending); // ��������
		}break;
		}
	}

	AllDelLink();
	printf("\n�ƹ�Ű�� �����ø� ����˴ϴ�.");
	_getch();
}