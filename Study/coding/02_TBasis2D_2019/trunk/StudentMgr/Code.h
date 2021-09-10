//(비연결리스트 사용버전)
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MAX_SAVE_FILE 100
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h> // rand(), srand(), system
#include <time.h> 
#include <string.h> 
#include <conio.h> // conio.h
#include <io.h>    // _finddata_t, _findfirst,_findnext
// 100명 번호,국,영,수 점수를 입력받고
// 번호, 총점, 평균을 출력하시오.(비연결리스트 사용버전)
typedef struct TStudentNode
{
	char m_szName[4];
	int  m_iIndex;
	int  m_iKor;
	int  m_iEng;
	int  m_iMat;
	int  m_iTotal;
	float  m_fAverage;
}Student;
// 로드 이후에 작업된 현 학생 데이터 정보
Student*	g_pStudentData = 0;
// 전체 학생 개수
int			g_iNumTotal = 0; 
// 파일검색 및 저장 배열
struct _finddata_t g_SaveFileArray[MAX_SAVE_FILE];
// 저장되어 있는 파일 개수
int			g_iNumSaveFile = 0;		
// 현재 작업 중인 파일이름
char		g_szCurrentFile[256] = { 0, };

enum WORK {
	DataSave,
	DataLoad,
	DataNew,
	DataPrint,
	DataFindIndex,
	DataFindName,
	SampleData = 9,
	Exit = 99,
};
// 초기 데이터 생성함수
void CreateSampleData();
// 로드 데이터 출력함수
void PrintData();
// 로드 데이터 인덱스로 검색 및 출력
void FindIndex();
// 로드 데이터 이름으로 검색 및 출력
void FindName();
// 메모리 데이터 저장
int FileSave();
// 파일에서 메모리로 로드
int FileLoad();
// 신규데이터를 파일에 출력 및 로드
void NewData();

void FindFile(void);
void SortFile(_finddata_t* c_file);

void CreateSampleData()
{
	/*printf("\n학생의 총 인원을 입력하시오 : ");
	scanf("%d", &g_iNumTotal);*/
	g_iNumTotal = 10;
	if (g_pStudentData != NULL)
	{
		free(g_pStudentData);
		g_pStudentData = 0;
	}
	if (g_pStudentData == NULL)
	{
		g_pStudentData = (Student*)malloc(sizeof(Student)* g_iNumTotal);
	}
	for (int i = 0; i < g_iNumTotal; i++)
	{
		g_pStudentData[i].m_szName[0] = 65 + rand() % 26;
		g_pStudentData[i].m_szName[1] = 65 + rand() % 26;
		g_pStudentData[i].m_szName[2] = 65 + rand() % 26;
		g_pStudentData[i].m_szName[3] = '\0';

		g_pStudentData[i].m_iIndex = i;
		g_pStudentData[i].m_iKor = rand() % 101;
		g_pStudentData[i].m_iEng = rand() % 101;
		g_pStudentData[i].m_iMat = rand() % 101;

		g_pStudentData[i].m_iTotal = g_pStudentData[i].m_iKor +
			g_pStudentData[i].m_iEng +
			g_pStudentData[i].m_iMat;
		g_pStudentData[i].m_fAverage = g_pStudentData[i].m_iTotal / 3.0f;
	}
	PrintData();
}
void PrintData()
{
	if (g_pStudentData == 0)
	{
		printf("\n데이터가 없습니다. 입력하시오!");
		return;
	}
	system("cls");
	printf("\n=====================================================");
	printf("\n====================성적관리프로그램=================");
	printf("\n=====================================================");
	printf("\n번호\t이름\t국어\t영어\t수학\t총점\t평균");
	printf("\n-----------------------------------------------------");
	for (int i = 0; i < g_iNumTotal; i++)
	{
		printf("\n%3d %8s %7d %7d %7d %7d %10.4f",
			g_pStudentData[i].m_iIndex,
			g_pStudentData[i].m_szName,
			g_pStudentData[i].m_iKor,
			g_pStudentData[i].m_iEng,
			g_pStudentData[i].m_iMat,
			g_pStudentData[i].m_iTotal,
			g_pStudentData[i].m_fAverage);
	}
}
void FindIndex()
{
	int findNumber = -1;
	system("cls");
	printf("\n 번호를 입력하시오 : ");
	scanf("%d", &findNumber);
	if (findNumber >= 0)
	{
		printf("\n%3d %8s %7d %7d %7d %7d %10.4f",
			g_pStudentData[findNumber].m_iIndex,
			g_pStudentData[findNumber].m_szName,
			g_pStudentData[findNumber].m_iKor,
			g_pStudentData[findNumber].m_iEng,
			g_pStudentData[findNumber].m_iMat,
			g_pStudentData[findNumber].m_iTotal,
			g_pStudentData[findNumber].m_fAverage);
	}
}
void FindName()
{
	char findName[5] = { 0, };
	printf("\n 이름을 입력하시오 : ");
	scanf("%s", &findName);
	for (int i = 0; i < g_iNumTotal; i++)
	{
		if (0 != strcmp(g_pStudentData[i].m_szName, findName))
		{
			continue;
		}
		printf("\n%3d %8s %7d %7d %7d %7d %10.4f",
			g_pStudentData[i].m_iIndex,
			g_pStudentData[i].m_szName,
			g_pStudentData[i].m_iKor,
			g_pStudentData[i].m_iEng,
			g_pStudentData[i].m_iMat,
			g_pStudentData[i].m_iTotal,
			g_pStudentData[i].m_fAverage);
		break;
	}
}
int FileSave()
{
	FILE* fp;
	time_t now = time(NULL);
	tm *lt = localtime(&now);
	char buffer[256] = { 0, };
	strftime(buffer, 256, "%Y년%m월%d일%H시%M분%S초.txt", lt);	

	fp = fopen(buffer, "w");
	if (fp == NULL)
	{
		return -1;
	}
	fprintf(fp, "%d", g_iNumTotal);
	for (int i = 0; i < g_iNumTotal; i++)
	{
		fprintf(fp, "\n%3d %8s %7d %7d %7d %7d %10.4f",
			g_pStudentData[i].m_iIndex,
			g_pStudentData[i].m_szName,
			g_pStudentData[i].m_iKor,
			g_pStudentData[i].m_iEng,
			g_pStudentData[i].m_iMat,
			g_pStudentData[i].m_iTotal,
			g_pStudentData[i].m_fAverage);
	}
	memset(g_szCurrentFile, 0, sizeof(char) * 256);
	strcpy(g_szCurrentFile, buffer);
	fclose(fp);
	return 1;
}
// 최근에 저장한 파일로 부터 데이터를 로드한다.
int FileLoad()
{	
	FILE* fp;
	FindFile();
	// 저장파일이 없다.
	if (g_iNumSaveFile < 1)
	{
		return -1;
	}

	// 파일포인터 생성
	fp = fopen( g_SaveFileArray[0].name, "r");
	if (fp == NULL)
	{	
		return -1;
	}
	// 기존 메모리 데이터 삭제 후 로드한다.
	if (g_pStudentData != NULL)
	{
		free(g_pStudentData);
		g_pStudentData = NULL;
	}

	fscanf(fp, "%d", &g_iNumTotal);
	if (g_pStudentData == NULL)
	{
		g_pStudentData = (Student*)malloc(sizeof(Student)* g_iNumTotal);
	}

	for (int i = 0; i < g_iNumTotal; i++)
	{
		fscanf(fp, "%d %s %d %d %d %d %f",
			&g_pStudentData[i].m_iIndex,
			g_pStudentData[i].m_szName,
			&g_pStudentData[i].m_iKor,
			&g_pStudentData[i].m_iEng,
			&g_pStudentData[i].m_iMat,
			&g_pStudentData[i].m_iTotal,
			&g_pStudentData[i].m_fAverage);
	}
	fclose(fp);
	PrintData();
	return 1;
}
void NewData()
{
	printf("\n신규 학생의 데이터를 입력하시오 : ");
	Student newData;
	printf("\n이름 :  ");	scanf("%s", newData.m_szName);
	printf("\n국어 :  ");	scanf("%d", &newData.m_iKor);
	printf("\n수학 :  ");	scanf("%d", &newData.m_iMat);
	printf("\n영어 :  ");	scanf("%d", &newData.m_iEng);
	newData.m_iTotal = newData.m_iKor + newData.m_iMat + newData.m_iEng;
	newData.m_fAverage = newData.m_iTotal / 3.0f;
	newData.m_iIndex = g_iNumTotal;
	// 현재 메모리 데이터를 먼저 저장 한다.	
	FileSave();

	// 이후 신규 데이터를 파일의 끝에 추가 저장한다.
	FILE* fp = fopen(g_szCurrentFile, "r+");
	if (fp == NULL)
	{
		if (fp == NULL) return;
	}
	fprintf(fp, "%d", g_iNumTotal + 1);
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "\n%3d %8s %7d %7d %7d %7d %10.4f",
		newData.m_iIndex,
		newData.m_szName,
		newData.m_iKor,
		newData.m_iEng,
		newData.m_iMat,
		newData.m_iTotal,
		newData.m_fAverage);
	fclose(fp);


	FileLoad();
}

void FindFile(void)
{
	memset(g_SaveFileArray, 0, sizeof(_finddata_t)*g_iNumSaveFile);
	g_iNumSaveFile = 0;

	struct _finddata_t c_file;
	intptr_t hFile;

	if ((hFile = _findfirst("*.txt", &c_file)) == -1L)
	{
		printf("No files in current directory!\n");
	}
	else
	{

		printf("Listing of files\n\n");
		printf("\nNOR RDO HID SYS ARC SUB dir FILE DATE %25c SIZE\n", ' ');
		printf("--- --- --- --- --- ---- ---- %25c ----\n", ' ');
		/* Find the rest of the .c files */
		do
		{
			char buffer[30];
			printf((c_file.attrib & _A_NORMAL) ? " Y " : " N ");	// 일반파일
			printf((c_file.attrib & _A_RDONLY) ? " Y " : " N ");	// 읽기전용
			printf((c_file.attrib & _A_SYSTEM) ? " Y " : " N ");	// 시스템파일
			printf((c_file.attrib & _A_HIDDEN) ? " Y " : " N ");	// 숨김파일
			printf((c_file.attrib & _A_ARCH) ? " Y " : " N ");		// 보관파일(파일을 변경 및 지울때 사용됨)
			printf((c_file.attrib & _A_SUBDIR) ? " Y " : " N ");	// 하위디렉토리

			ctime_s(buffer, _countof(buffer), &c_file.time_write);

			printf(" %-12s %.24s %9ld\n",
				c_file.name, ctime(&(c_file.time_write)), c_file.size);
			
			SortFile(&c_file);
			
		} while (_findnext(hFile, &c_file) == 0);

		_findclose(hFile);
	}
}
// 최근 생성한 일자를 기준으로 앞에서 부터 정렬된다.
void SortFile(_finddata_t* c_file)
{
	// 최대 100개만 저장한다.
	if (g_iNumSaveFile >= MAX_SAVE_FILE)
	{		
		return;
	}

	_finddata_t file;
	int iIndex = 0;
	for (iIndex = 0; iIndex < g_iNumSaveFile; iIndex++)
	{
		memcpy(&file, &g_SaveFileArray[iIndex], sizeof(_finddata_t));
		if (file.time_write < c_file->time_write)
		{			
			break;
		}
	}
	for (int iCnt = g_iNumSaveFile; iCnt > iIndex; iCnt--)
	{
		memcpy(	&g_SaveFileArray[iCnt],
				&g_SaveFileArray[iCnt-1], sizeof(_finddata_t));		
	}
	memcpy(&g_SaveFileArray[iIndex], c_file, sizeof(_finddata_t));
	g_iNumSaveFile++;
}