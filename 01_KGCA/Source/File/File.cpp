#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <vector>
struct User
{
	int		iIndex;
	int		iAge;
};
int main()
//r 읽기 r+ 읽기 쓰기 w 쓰기 w+ 읽기쓰기 a 덧붙이기 a+ 읽기 덧붙이기
{
	User	name;
	name.iIndex = 1;
	name.iAge = 2;
	//	파일 생성 저장
	FILE*	fpWrite = fopen("test.txt", "w");

	fprintf(fpWrite, "%d %d", name.iIndex, name.iAge);
	fclose(fpWrite);


	// 파일 읽기
	//char	buffer[256] = { 0, };
	int		tempIndex = 0;
	int		tempiAge = 0;
	FILE*	fpRead = fopen("test.txt", "r");
	//fgets(buffer, 256, fpRead);
	fscanf(fpRead, "%d %d", &tempIndex, &tempiAge);
	printf("%d %d",tempIndex, tempiAge);
	fclose(fpRead);
	return 0;
}
