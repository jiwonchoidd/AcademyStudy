#include <stdio.h>
// char -128 ~ +127 범위를 갖는다.
void main()
{
	char a;
	char b;
	char c;
	char d;
	a = 127;
	b = 128;
	c = -128;
	d = -129;
	printf("\n%d %d %d %d ", a, b, c, d);	

	// 0 ~ 255 범위
	unsigned char e;
	unsigned char f;
	unsigned char g;
	unsigned char h;
	e = 0;
	f = 255;
	g = -1;
	h = 256;
	printf("\n%d %d %d %d ", e, f, g, h);	
}