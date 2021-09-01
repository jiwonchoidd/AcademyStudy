/*
	bool1.cpp
	C 스타일의 프로그램.
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

	printf("숫자 입력 : ");
	scanf("%d", &num);
	
	result=IsPositive(num);
	if(result==TRUE)
		printf("Positive number \n");
	else
		printf("Negative number \n");

	return 0;
}