#include <stdio.h>
// 아스키코드값 0 ~ 126까지 문자화 되어있다.
void main()
{
	for (int i = 65; i <= 90; i++)
	{
		printf("%c", i);
	}
	printf("\n");
	for (int i = 97; i <= 122; i++)
	{
		printf("%c", i);
	}

	printf("\n%s", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	//문자열 끝에는 NULL문자가 있어야 한ㄷ.
	//알파벳개수 + 1(널문자)= 27
	char alphabat[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	printf("\n%s", alphabat);
	printf("\n");

	// 문자열의 모든 문자 단위로 출력한다.
	for (int i = 0; i < 27; i++)
	{
		printf("%c", alphabat[i]);
	}	
}