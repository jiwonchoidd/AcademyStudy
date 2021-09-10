#include <stdio.h>
// 아스키코드값 0 ~ 126까지 문자화 되어있다.
void main()
{
	/*for (int i = 0; i < 127; i++)
	{
		printf("%d", i);
		printf("\t%c ", i);		
	}	
	printf("\n");*/
	// 10의 정수를 2진수로 출력
	int iValue = 10;
	for ( int j=7; j>=0; --j)
	{		
		printf("%d", (iValue >>j) & 1); // >>, << 사프트 연산자는 실수에서 사용 할 수 없다.
	}
	// 음수의 표현에서는 2의 보수를 사용하여 표현한다.
	// -0을 표현할 수 없으나 -128를 표현할 수 있다.
	iValue = -10;
	for (int j = 7; j >= 0; --j)
	{
		printf("%d", (iValue >> j) & 1); // >>, << 사프트 연산자는 실수에서 사용 할 수 없다.
	}
}