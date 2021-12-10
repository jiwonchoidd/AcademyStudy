#include "FileIO.h"

void FileIO::FileCreate()
{	
	int a = 80, b=70 , c=90;
	FILE* fpWrite = fopen("Grade Manager.txt", "w");
	fprintf(fpWrite, "이름		국어		수학		영어		총점		평균");
	fprintf(fpWrite,"%s %d %d %d %d %d", "최지원");
	fclose(fpWrite);
}


bool FileIO::FileLoad()
{
	int		tempIndex = 0;
	int		tempiAge = 0;
	FILE* fpRead = fopen("Grade Manager.txt", "rb");
	//파일의 유무를 따진다.
	if (fpRead == NULL)
	{
		return false;
	}
	else
	{
		char str[256] = {0,};
		fscanf(fpRead, "%s", str[0]);
		printf("%s%s", str[0]);
		fclose(fpRead);
		return true;
	}
}

void FileIO::FileSave()
{
}

void FileIO::FileDeleteAll()
{
}
void FileIO::FileDraw()
{
}