/*
	bool1.cpp
	C ��Ÿ���� ���α׷�.
*/

#include <stdio.h>

const int TRUE=1;
const int FALSE=0;

int IsPositive(int i)
{
	if(i<0)
		return FALSE;
	else
		return TRUE;
}

int main(void)
{
	int num;
	int result;

	printf("���� �Է� : ");
	scanf("%d", &num);
	
	result=IsPositive(num);
	if(result==TRUE)
		printf("Positive number \n");
	else
		printf("Negative number \n");

	return 0;
}