// Project10.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <conio.h>
int g_iValue = 100;

int main()
{
	while(1)
	{
		if (_kbhit() == 1)
		{
			_getch();
			printf("%d", g_iValue);
		}
	}
	return 0;
}
