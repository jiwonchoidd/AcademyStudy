#include <stdio.h>

void main()
{
	float a;
	float b;
	a = 3.14f;
	b = -3.14f;

	float c = a + b;
	printf("\n%f + %f =  %f", a, b, c);


	float d = 3.14f;
	float e = -3.14f;
	float f = d + e;
	printf("\n%f + %f =  %f", d, e, f);


	double aValue = 3.14;
	double bValue = -3.14;
	double cValue = aValue + bValue;
	printf("\n%lf + %lf =  %lf", 
		aValue, bValue, cValue);
	printf("\n%e + %e =  %e", 
		aValue, bValue, cValue);

}