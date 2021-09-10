#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
void DrawChar(char cValue);
void DrawStar(int iMaxRow, int iMaxColumn, int iStartColumn, int iDir )
{
	for (int iRow = 0; iRow < iMaxRow; iRow++)
	{
		int iNumCol = iMaxColumn - (iRow * iDir);
		for (int iCol = 0; iCol < iNumCol; iCol++)
		{
			DrawChar('-');
		}
		iNumCol = (iStartColumn - (iRow * -iDir)) * 2 + 1;
		for (int iCol = 0; iCol < iNumCol; iCol++)
		{
			DrawChar('*');
		}
		printf("\n");
	}
}
void DrawChar(char cValue )
{
	fprintf(stdout,"%c", cValue);
}
// diamond
void DrawDiamond(int iNumRow,	int iNumCol)
{
	//iDir=1 �Ʒ��� ���� ����, iDir=-1 �Ʒ��� ���� ����
	DrawStar(iNumRow,     iNumCol, 0,       1);
	DrawStar(iNumRow + 1, 0,       iNumCol, -1);
}

void main()
{
	int iNumColumn = 7;
	int iNumRow = 7;
	printf("���� �� ���� �Է�\n");
	scanf("%d", &iNumColumn);
	scanf("%d", &iNumRow);
	// 1)���� ����� ����Ͽ� ���
	for (int row = 0; row < iNumRow; row++)
	{
		for (int col = 0; col < iNumColumn-row; col++)
		{
			printf(" ");
		}
		for (int k =0; k < 2*row+1; k++)
		{
			printf("*");
		}
		printf("\n");
	}
	for (int row = 0; row < iNumRow+1; row++)
	{
		for (int col = 0; col < row; col++)
		{
			printf(" ");
		}
		for (int k = 0; k < 2*(iNumColumn-row)+1; k++)
		{
			printf("*");
		}
		printf("\n");
	}

	// 2)�Լ��� ����Ͽ� ���
	DrawDiamond(iNumRow, iNumColumn);
}