#include <stdio.h>

void main()
{
	int iTotal = 0;
	// for��
	for( int iCount=0; iCount <= 10; iCount++)
	{
		iTotal += iCount;
		printf("\n 0~%d������ �հ�� %d �Դϴ�.", iCount,iTotal);
	}
	// while��
	iTotal = 0;
	int jCount = 0;
	while (jCount <= 10)
	{
		iTotal += jCount;
		printf("\n 0~%d������ �հ�� %d �Դϴ�.", jCount++, iTotal);
	}

	// do ~ while��
	iTotal = 0;
	jCount = 0;
	do
	{
		iTotal += jCount;
		printf("\n 0~%d������ �հ�� %d �Դϴ�.", jCount, iTotal);
	} while (jCount++ < 10);
}