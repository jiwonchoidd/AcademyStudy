#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time(NULL)

void main()
{
	int iValue = 0;
	int iCount = 0;

	srand(time(NULL));
	// 무한 for문
	for (; ; ) 
	{
		iValue = rand() % 101;
		if (iValue == 77) 
		{
			printf("\n %d번 반복되었다.", iCount);
			break;
		}
		iCount++;
	}
	// 무한 while문
	iCount = 0;
	while (1)
	{
		iValue = rand() % 101;
		if (iValue == 77)
		{
			printf("\n %d번 반복되었다.", iCount);
			break;
		}
		iCount++;
	}

	// 무한 do ~ while문
	iCount = 0;
	do
	{
		iValue = rand() % 101;
		if (iValue == 77)
		{
			printf("\n %d번 반복되었다.", iCount);
			break;
		}
		iCount++;
	} while (1);
}