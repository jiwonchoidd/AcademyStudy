//////////////////////  ���/�ݺ��� /////////////////////////
// ����� ���� �Լ����
// while()
// if ~ else
// for()
//////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> // cstdio
#include <stdlib.h>//cstdlib
#include <conio.h>//_getch()
#define USE_ARRAY

#ifdef USE_ARRAY
int Data[20][20];
///////////////////////////////////////////////////////////////////
// ũ�Ⱑ iSize�� ���������� �����.
///////////////////////////////////////////////////////////////////
void MagicSquare(int iSize)
{
	int iWidth, iHeight;
	int suja;

	// 0���� �߰��� 1�� ä���.
	int iRow = 0;
	int	iCol = (iSize - 1) / 2;
	Data[0][iCol] = 1;

	for (suja = 2; suja <= iSize * iSize; suja++)
	{
		// ���� ���� ���� �����̸� 
		if (iRow == 0)		iHeight = iSize - 1;		// ���� �Ʒ�������
		else				iHeight = iRow - 1;		// �ϳ� �������� �̵�

													// ��� 1
													// ���� ���� ���� ������ ���̸�
		if (iCol == iSize - 1)	iWidth = 0;				// ���� ���� ����
		else				iWidth = iCol + 1;		// �ϳ� ������ ���� �̵�

													// ��� 2
													//	// ���� ���� ���� ���� ���̸�
													//	if( iCol ==0 )		iWidth = iSize-1;	// ���� ������ ����
													//	else				iWidth = iCol-1;	// �ϳ� ���� ���� �̵�

													// �̵����� ��ġ�� ��ĭ�̸�
		if (Data[iHeight][iWidth] == 0)
		{
			iRow = iHeight, iCol = iWidth;			// �� ��ġ�� ���ڸ� ä�� ��ġ�� �Ѵ�.
		}
		else
		{
			iRow++;									// ��ĭ�� �ƴϸ� �Ʒ������� �̵�
		}
		Data[iRow][iCol] = suja;					// ���ڸ� ä���.
	}
}
///////////////////////////////////////////////////////////////////
// �������� ũ�⸦ �Է¹ް� �������� ����Ѵ�.
///////////////////////////////////////////////////////////////////
void main()
{
	int iSize;

	// ������ �Է�
	for (;;)
	{
		printf("������ ũ��( 3 ~ 19���� Ȧ��)....?");
		scanf("%d", &iSize);
		if (iSize >= 3 && iSize < 20)
		{
			break;
		}

	}

	// ������ ���
	MagicSquare(iSize);

	// ������ ���
	for (int iRow = 0; iRow < iSize; iRow++)
	{
		for (int jCol = 0; jCol < iSize; jCol++)
		{
			printf("%4d", Data[iRow][jCol]);
		}
		printf("\n");
	}
	_getch();
}
#else
// �Ʒ� ���Լ��� �����Ҵ� ó������ ���.
void Collision(int *x, int *y);
int DynamicAlloc(int iSize);
void Print(int* pData, int iSize)
{
	system("cls");
	for (int iRow = 0; iRow < iSize; iRow++)
	{
		printf("\n");

		for (int iCol = 0; iCol < iSize; iCol++)
		{
			int iIndex = iRow * iSize + iCol;
			printf("%d\t", pData[iIndex]);
		}
	}
}
void Collision(int *x, int *y)
{
	*x = *x - 1;
	*y = *y + 1;
	*y = *y + 1;
}
int DynamicAlloc(int iSize)
{
	int  *pData = (int*)calloc(iSize*iSize, sizeof(int));
	int y = 0;
	int x = iSize / 2;
	int iIndex = y * iSize + x;
	int iCount = 1;

	pData[iIndex] = iCount++;
	while (iCount < (iSize*iSize + 1))
	{
		x = x + 1;
		y = y - 1;
		if (x == iSize && y < 0)
		{
			Collision(&x, &y);
		}
		else
		{
			if (x == iSize)
			{
				x = 0;
			}
			if (y < 0) {
				y = iSize - 1;
			}
		}

		iIndex = y * iSize + x;
		if (pData[iIndex] != 0)
		{
			Collision(&x, &y);
			iIndex = y * iSize + x;
		}
		pData[iIndex] = iCount++;
	}

	//3) ���
	Print(pData, iSize);
	free(pData);
	return 0;
}
///////////////////////////////////////////////////////////////////
// �������� ũ�⸦ �Է¹ް� �������� ����Ѵ�.
///////////////////////////////////////////////////////////////////
void main()
{
	int iSize;

	// ������ �Է�
	for (;;)
	{
		printf("������ ũ��( 3 ~ 19���� Ȧ��)....?");
		scanf("%d", &iSize);
		if (iSize >= 3 && iSize < 20)
		{
			break;
		}
	}
	// �����Ҵ� ���
	DynamicAlloc(iSize);
	_getch();
}
#endif