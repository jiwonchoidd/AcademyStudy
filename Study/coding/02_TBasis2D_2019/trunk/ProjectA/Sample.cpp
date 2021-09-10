#include <stdio.h>

void main()
{
	int iTotal = 65;
	// if문
	if (0<=iTotal <= 100)
	{
		printf("\n%d점수는 유효한 점수입니다.", iTotal);
	}
	// if~else 문
	if (iTotal >= 60)
	{
		printf("\n60[%d]점수 이상은 합격 점수입니다.", iTotal);
	}else
	{
		printf("\n60[%d]점수 미만은 불합격 점수입니다.", iTotal);
	}
	// 다중 if-else
	if (iTotal >= 90)
	{
		printf("\n%d점수 이상은 A학점입니다.", iTotal);
	}
	else if (iTotal >= 80)
	{
		printf("\n%d점수는 B학점입니다.", iTotal);
	}
	else if (iTotal >= 70)
	{
		printf("\n%d점수는 C학점입니다.", iTotal);
	}
	else if (iTotal >= 60)
	{
		printf("\n%d점수는 D학점입니다.", iTotal);
	}
	else 
	{
		printf("\n%d점수는 F학점입니다.", iTotal);
	}
	// switch-case
	// 다중 if-else
	int iCredit = iTotal / 10;
	switch (iCredit)
	{
		case 9:
		{
			printf("\n%d점수는 A학점입니다.", iTotal);
		}break;
		case 8:
		{
			printf("\n%d점수는 B학점입니다.", iTotal);
		}break;
		case 7:
		{
			printf("\n%d점수는 C학점입니다.", iTotal);
		}break;
		case 6:
		{
			printf("\n%d점수는 D학점입니다.", iTotal);
		}break;
		default:
		{
			printf("\n%d점수는 F학점입니다.", iTotal);
		}
	}
}