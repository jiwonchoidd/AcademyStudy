#include <stdio.h>
// 대략 -21억 ~ +21억
// int -2147483648 ~ +2147483647 범위를 갖는다.
void main()
{
	int a;
	int b;
	int c;
	a = 2147483647; //21억
	b = 2147483648; 
	c = a + b;
	printf("\n%d %d %d", a, b, c);
	printf("\na[%d]+b[%d]=c[%d]", a, b, c);

	// 대략 0 ~ +42억
	// int 0 ~ +4294967295 범위를 갖는다.
	unsigned int d;
	unsigned int e;
	unsigned int f;
	d = 4294967295; //21억
	e = 4294967296;
	f = a + b;
	printf("\n%d %d %d", d, e, f);
	printf("\na[%lu]+b[%lu]=c[%lu]", d, e, f);
}