//////////////////////  제어문/반복문 /////////////////////////
// 사용자 정의 함수사용
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
// 크기가 iSize인 마방진으로 만든다.
///////////////////////////////////////////////////////////////////
void MagicSquare(int iSize)
{
	int iWidth, iHeight;
	int suja;

	// 0행의 중간에 1을 채운다.
	int iRow = 0;
	int	iCol = (iSize - 1) / 2;
	Data[0][iCol] = 1;

	for (suja = 2; suja <= iSize * iSize; suja++)
	{
		// 현재 행이 제일 위행이면 
		if (iRow == 0)		iHeight = iSize - 1;		// 제일 아래행으로
		else				iHeight = iRow - 1;		// 하나 위행으로 이동

													// 방법 1
													// 현재 행이 제일 오른쪽 열이면
		if (iCol == iSize - 1)	iWidth = 0;				// 제일 왼쪽 열로
		else				iWidth = iCol + 1;		// 하나 오른쪽 열로 이동

													// 방법 2
													//	// 현재 행이 제일 왼쪽 열이면
													//	if( iCol ==0 )		iWidth = iSize-1;	// 제일 오른쪽 열로
													//	else				iWidth = iCol-1;	// 하나 왼쪽 열로 이동

													// 이동후의 위치가 빈칸이면
		if (Data[iHeight][iWidth] == 0)
		{
			iRow = iHeight, iCol = iWidth;			// 이 위치를 숫자를 채울 위치로 한다.
		}
		else
		{
			iRow++;									// 빈칸이 아니면 아래행으로 이동
		}
		Data[iRow][iCol] = suja;					// 숫자를 채운다.
	}
}
///////////////////////////////////////////////////////////////////
// 마방진의 크기를 입력받고 마방진을 계산한다.
///////////////////////////////////////////////////////////////////
void main()
{
	int iSize;

	// 마방진 입력
	for (;;)
	{
		printf("마방진 크기( 3 ~ 19사이 홀수)....?");
		scanf("%d", &iSize);
		if (iSize >= 3 && iSize < 20)
		{
			break;
		}

	}

	// 마방진 계산
	MagicSquare(iSize);

	// 마방진 출력
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
// 아래 두함수는 동적할당 처리에서 사용.
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

	//3) 출력
	Print(pData, iSize);
	free(pData);
	return 0;
}
///////////////////////////////////////////////////////////////////
// 마방진의 크기를 입력받고 마방진을 계산한다.
///////////////////////////////////////////////////////////////////
void main()
{
	int iSize;

	// 마방진 입력
	for (;;)
	{
		printf("마방진 크기( 3 ~ 19사이 홀수)....?");
		scanf("%d", &iSize);
		if (iSize >= 3 && iSize < 20)
		{
			break;
		}
	}
	// 동적할당 방법
	DynamicAlloc(iSize);
	_getch();
}
#endif