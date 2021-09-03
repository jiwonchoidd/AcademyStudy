#include <stdio.h>

void main()
{
	int iTotal = 0;
	// for문
	for( int iCount=0; iCount <= 10; iCount++)
	{
		iTotal += iCount;
		printf("\n 0~%d까지의 합계는 %d 입니다.", iCount,iTotal);
	}
	// while문
	iTotal = 0;
	int jCount = 0;
	while (jCount <= 10)
	{
		iTotal += jCount;
		printf("\n 0~%d까지의 합계는 %d 입니다.", jCount++, iTotal);
	}

	// do ~ while문
	iTotal = 0;
	jCount = 0;
	do
	{
		iTotal += jCount;
		printf("\n 0~%d까지의 합계는 %d 입니다.", jCount, iTotal);
	} while (jCount++ < 10);
}