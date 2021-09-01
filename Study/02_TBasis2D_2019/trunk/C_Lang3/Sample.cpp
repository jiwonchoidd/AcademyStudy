#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>//<stdio.h>
#include <cstdlib>//<stdlib.h>
#include <cstring>//<string.h>
#include <malloc.h> // _msize

// SDL(security development lifecycle) ����ũ�μ���Ʈ ���Ȱ��� ����������Ŭ(���ȿ� ������ ����)
// �Ӽ�->C/C++->  SDL -> �ƴϿ�(/sdl-) �����Ͽ� ������ �Ѵ�.
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
	�޸� ����(code(�ؽ�Ʈ,���,���), data(�����޸�), stack(���ø޸�), heap(�����޸�))
	char *pData = "kgca";     //data area save:��������(�迭),��������(�迭),����ü = > ����� ����
	char pData[]= "kgca";     //stack area save:��������=>������
	char *pData = (char *)malloc(sizeof(char)*10);
	str = "kgca";             //heap area save:����� ����
*/
// str1�� ũ�� >= st1+str2 �̾�� �Ѵ�.
char* StringCat(char* str1, char* str2)
{
	int iDestSize = 0;// strlen(str1);
	int iSrcSize = 0;// strlen(str2);
	
	// static char iTemp[1024] = {0,}; // �����忡 unsafe
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

	printf("\========= ���ڿ� ����      ===========\n");
	strcpy( str1, data);
	strncpy(str2, "KGCA GAME ACADEMY", 4);	// �ι��ڰ� ���� ������ �����Ⱑ ��µȴ�.

	printf("str1 : %s\n", str1);
	printf("str2 : %s\n", str2);

	str2[4] = 0;
	printf("str2 : %s\n", str2);

	printf("\========= ���ڿ� ����/�� ===========\n"); 
	printf("strlen(str1): %u\n", strlen(str1));
	printf("strcmp(str1, str2): %d\n", strcmp(str1, str2));
	printf("strcmp(str2, str1): %d\n", stricmp(str2, str1));
	printf("strncmp(str1, str2, 4): %d\n", strncmp(str1, str2, 4));

	printf("\========= ���ڿ� ��ȯ      ===========\n"); 
	printf("str1 Capital Letter: %s\n", strupr(str1));
	printf("str1 Default: %s\n", str1);
	printf("str1 Small Letter: %s\n", strlwr(str1));
	printf("str1 Default: %s\n", str1);

	printf("\========= ���ڿ� ����      ===========\n"); 
	printf("str1 %s\n", strchr(str1, 'g'));
	if( strchr(str1, 'A' ) != 0 )
	{
		printf("\nstrchr : %s\n", strchr(str1, 'g'));
	}	
	printf("\nstrstr :  %s\n", strstr(str1, "academy"));
	

	printf("\========= ���ڿ� �߰�      ===========\n"); 
	char buffer[100] = { 0, };
	strcpy(buffer, str1);
	printf("strcat   :  %s\n", strcat(buffer, str2));
	printf("str1     :  %s\n", str1);
	printf("str2     :  %s\n", str2);
	free(str1);

	// buffer�� buffer+str2�� ũ�⸦ ������ ��ŭ Ŀ�߸� �Ѵ�.
	char *pLink = StringCat(buffer, str2);
	printf("str1+atr2:  %s\n", pLink);

	// �ű� ���۸� �Ҵ��Ͽ� ��ȯ�ϱ� ������ �޸� �����ؾ� �Ѵ�.
	char *p = StringToAttach(buffer, str2);
	printf("str1+atr2=%d:  %s\n", _msize(p),p);
	
	
	printf("\========= ���ڿ� �и�      ===========\n"); 
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