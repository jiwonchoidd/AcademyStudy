#include <stdio.h>
// char -128 ~ + 127 범위를 갖는다.
void main()
{
	printf("\n%c %c", 'A', 'a');
	printf("\n%d %d", 'A', 'a');

	char value_A = 'A';
	char vlaue_a = 'a';
	printf("\n%c %c", value_A, vlaue_a);
	printf("\n%d %d", value_A, vlaue_a);
	
}