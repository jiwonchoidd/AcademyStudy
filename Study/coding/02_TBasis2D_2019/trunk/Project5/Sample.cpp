#include <stdio.h>
// short -32768 ~ +32767 범위를 갖는다.
void main()
{
	short a;
	short b;
	short c;
	short d;
	a = -32768;
	b = -32769;
	c = 32767;
	d = 32768;
	printf("\n%d %d %d %d ", a, b, c, d);

	//unsigned short 0 ~ 65535 의 범위를 갖는다.
	unsigned short e;
	unsigned short f;
	unsigned short g;
	unsigned short h;
	e = 0;
	f = 65535;
	g = -1;
	h = 65536;
	printf("\n%u %u %u %u ", e, f, g, h);
}