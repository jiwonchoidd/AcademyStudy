#include <stdio.h>
#include <stdlib.h> // rand()
#include <time.h> // time(NULL)

void main()
{
	int iValue = 0;
	int iCount = 0;

	srand(time(NULL));
	// ���� for��
	for (; ; ) 
	{
		iValue = rand() % 101;
		if (iValue == 77) 
		{
			printf("\n %d�� �ݺ��Ǿ���.", iCount);
			break;
		}
		iCount++;
	}
	// ���� while��
	iCount = 0;
	while (1)
	{
		iValue = rand() % 101;
		if (iValue == 77)
		{
			printf("\n %d�� �ݺ��Ǿ���.", iCount);
			break;
		}
		iCount++;
	}

	// ���� do ~ while��
	iCount = 0;
	do
	{
		iValue = rand() % 101;
		if (iValue == 77)
		{
			printf("\n %d�� �ݺ��Ǿ���.", iCount);
			break;
		}
		iCount++;
	} while (1);
}