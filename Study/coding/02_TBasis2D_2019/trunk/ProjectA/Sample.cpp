#include <stdio.h>

void main()
{
	int iTotal = 65;
	// if��
	if (0<=iTotal <= 100)
	{
		printf("\n%d������ ��ȿ�� �����Դϴ�.", iTotal);
	}
	// if~else ��
	if (iTotal >= 60)
	{
		printf("\n60[%d]���� �̻��� �հ� �����Դϴ�.", iTotal);
	}else
	{
		printf("\n60[%d]���� �̸��� ���հ� �����Դϴ�.", iTotal);
	}
	// ���� if-else
	if (iTotal >= 90)
	{
		printf("\n%d���� �̻��� A�����Դϴ�.", iTotal);
	}
	else if (iTotal >= 80)
	{
		printf("\n%d������ B�����Դϴ�.", iTotal);
	}
	else if (iTotal >= 70)
	{
		printf("\n%d������ C�����Դϴ�.", iTotal);
	}
	else if (iTotal >= 60)
	{
		printf("\n%d������ D�����Դϴ�.", iTotal);
	}
	else 
	{
		printf("\n%d������ F�����Դϴ�.", iTotal);
	}
	// switch-case
	// ���� if-else
	int iCredit = iTotal / 10;
	switch (iCredit)
	{
		case 9:
		{
			printf("\n%d������ A�����Դϴ�.", iTotal);
		}break;
		case 8:
		{
			printf("\n%d������ B�����Դϴ�.", iTotal);
		}break;
		case 7:
		{
			printf("\n%d������ C�����Դϴ�.", iTotal);
		}break;
		case 6:
		{
			printf("\n%d������ D�����Դϴ�.", iTotal);
		}break;
		default:
		{
			printf("\n%d������ F�����Դϴ�.", iTotal);
		}
	}
}