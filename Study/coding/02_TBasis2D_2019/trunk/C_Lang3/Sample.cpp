#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>//<stdio.h>
#include <cstdlib>//<stdlib.h>
#include <cstring>//<string.h>
#include <malloc.h> // _msize

// SDL(security development lifecycle) 마이크로소프트 보안개발 라이프사이클(보안에 안전한 개발)
// 속성->C/C++->  SDL -> 아니요(/sdl-) 선택하여 컴파일 한다.
char* StringToAttach( char* str1, char* str2 )
{
	int iDestSize = 0;// strlen(str1);
	int iSrcSize = 0;// strlen(str2);

	while (str1[iDestSize++] != 0);
	while (str2[iSrcSize++] != 0);

	char *pStrReturn = (char*)malloc(iDestSize + iSrcSize);

	strcpy(pStrReturn, str1 );
	pStrReturn += iDestSize-1;

	strcpy( pStrReturn, str2);
	pStrReturn -= iDestSize-1;
	return pStrReturn;
}
/*
	메모리 영역(code(텍스트,제어문,상수), data(정적메모리), stack(스택메모리), heap(동적메모리))
	char *pData = "kgca";     //data area save:전역변수(배열),정적변수(배열),구조체 = > 종료시 삭제
	char pData[]= "kgca";     //stack area save:지역변수=>블럭삭제
	char *pData = (char *)malloc(sizeof(char)*10);
	str = "kgca";             //heap area save:명시적 삭제
*/
// str1의 크기 >= st1+str2 이어야 한다.
char* StringCat(char* str1, char* str2)
{
	int iDestSize = 0;// strlen(str1);
	int iSrcSize = 0;// strlen(str2);
	
	// static char iTemp[1024] = {0,}; // 스레드에 unsafe
	while (str1[iDestSize++] != 0);
	while (str2[iSrcSize++] != 0);

	strcpy(&str1[iDestSize-1], str2);	
	return str1;
}
void main()
{
	char data[] = "k,g,c,a, game academy -36-";
	int iDataSize = strlen(data)+1;
	char *str1 = (char*)malloc(sizeof(char) * iDataSize);
	char str2[30];	

	printf("\========= 문자열 복사      ===========\n");
	strcpy( str1, data);
	strncpy(str2, "KGCA GAME ACADEMY", 4);	// 널문자가 없기 때문에 쓰레기가 출력된다.

	printf("str1 : %s\n", str1);
	printf("str2 : %s\n", str2);

	str2[4] = 0;
	printf("str2 : %s\n", str2);

	printf("\========= 문자열 길이/비교 ===========\n"); 
	printf("strlen(str1): %u\n", strlen(str1));
	printf("strcmp(str1, str2): %d\n", strcmp(str1, str2));
	printf("strcmp(str2, str1): %d\n", stricmp(str2, str1));
	printf("strncmp(str1, str2, 4): %d\n", strncmp(str1, str2, 4));

	printf("\========= 문자열 변환      ===========\n"); 
	printf("str1 Capital Letter: %s\n", strupr(str1));
	printf("str1 Default: %s\n", str1);
	printf("str1 Small Letter: %s\n", strlwr(str1));
	printf("str1 Default: %s\n", str1);

	printf("\========= 문자열 조사      ===========\n"); 
	printf("str1 %s\n", strchr(str1, 'g'));
	if( strchr(str1, 'A' ) != 0 )
	{
		printf("\nstrchr : %s\n", strchr(str1, 'g'));
	}	
	printf("\nstrstr :  %s\n", strstr(str1, "academy"));
	

	printf("\========= 문자열 추가      ===========\n"); 
	char buffer[100] = { 0, };
	strcpy(buffer, str1);
	printf("strcat   :  %s\n", strcat(buffer, str2));
	printf("str1     :  %s\n", str1);
	printf("str2     :  %s\n", str2);
	free(str1);

	// buffer는 buffer+str2의 크기를 저장할 만큼 커야만 한다.
	char *pLink = StringCat(buffer, str2);
	printf("str1+atr2:  %s\n", pLink);

	// 신규 버퍼를 할당하여 반환하기 때문에 메모리 해제해야 한다.
	char *p = StringToAttach(buffer, str2);
	printf("str1+atr2=%d:  %s\n", _msize(p),p);
	
	
	printf("\========= 문자열 분리      ===========\n"); 
	char * pch = p;
	printf ("Splitting [\"%s\"] into tokens:\n", p);
	char tok[] = " ,.-";
	pch = strtok(pch, tok);//, . - ");
	while (pch != NULL)
	{
		printf ("%s\n",pch);
		pch = strtok (NULL, tok);
	}		
	free(p);
	getchar();
}