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
{
	std::vector<User> vUser;
	vUser.push_back({ 1,2 });
	vUser.push_back({ 3,4 });
	vUser.push_back({ 5,6 });
	//	파일 생성 저장
	FILE* fpWrite = fopen("test.txt", "w");
	fclose(fpWrite);
	fprintf(fpWrite, "%s\n", vUser);
	if ((fpWrite = fopen("test.txt", "wt")) == NULL)
	{
		printf("파일열기 실패");
		return 1;
	}
	fprintf(fpWrite,"", vUser);
	fclose(fpWrite);

	FILE* fpRead = fopen("test.txt", "r");
	char szRead[10] = { 0, };
	fclose(fpWrite);
	return 0;
}
