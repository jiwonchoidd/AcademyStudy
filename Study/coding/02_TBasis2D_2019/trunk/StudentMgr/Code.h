//(�񿬰Ḯ��Ʈ ������)
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
// 100�� ��ȣ,��,��,�� ������ �Է¹ް�
// ��ȣ, ����, ����� ����Ͻÿ�.(�񿬰Ḯ��Ʈ ������)
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
// �ε� ���Ŀ� �۾��� �� �л� ������ ����
Student*	g_pStudentData = 0;
// ��ü �л� ����
int			g_iNumTotal = 0; 
// ���ϰ˻� �� ���� �迭
struct _finddata_t g_SaveFileArray[MAX_SAVE_FILE];
// ����Ǿ� �ִ� ���� ����
int			g_iNumSaveFile = 0;		
// ���� �۾� ���� �����̸�
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
// �ʱ� ������ �����Լ�
void CreateSampleData();
// �ε� ������ ����Լ�
void PrintData();
// �ε� ������ �ε����� �˻� �� ���
void FindIndex();
// �ε� ������ �̸����� �˻� �� ���
void FindName();
// �޸� ������ ����
int FileSave();
// ���Ͽ��� �޸𸮷� �ε�
int FileLoad();
// �űԵ����͸� ���Ͽ� ��� �� �ε�
void NewData();

void FindFile(void);
void SortFile(_finddata_t* c_file);

void CreateSampleData()
{
	/*printf("\n�л��� �� �ο��� �Է��Ͻÿ� : ");
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
		printf("\n�����Ͱ� �����ϴ�. �Է��Ͻÿ�!");
		return;
	}
	system("cls");
	printf("\n=====================================================");
	printf("\n====================�����������α׷�=================");
	printf("\n=====================================================");
	printf("\n��ȣ\t�̸�\t����\t����\t����\t����\t���");
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
	printf("\n ��ȣ�� �Է��Ͻÿ� : ");
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
	printf("\n �̸��� �Է��Ͻÿ� : ");
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
	strftime(buffer, 256, "%Y��%m��%d��%H��%M��%S��.txt", lt);	

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
// �ֱٿ� ������ ���Ϸ� ���� �����͸� �ε��Ѵ�.
int FileLoad()
{	
	FILE* fp;
	FindFile();
	// ���������� ����.
	if (g_iNumSaveFile < 1)
	{
		return -1;
	}

	// ���������� ����
	fp = fopen( g_SaveFileArray[0].name, "r");
	if (fp == NULL)
	{	
		return -1;
	}
	// ���� �޸� ������ ���� �� �ε��Ѵ�.
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
	printf("\n�ű� �л��� �����͸� �Է��Ͻÿ� : ");
	Student newData;
	printf("\n�̸� :  ");	scanf("%s", newData.m_szName);
	printf("\n���� :  ");	scanf("%d", &newData.m_iKor);
	printf("\n���� :  ");	scanf("%d", &newData.m_iMat);
	printf("\n���� :  ");	scanf("%d", &newData.m_iEng);
	newData.m_iTotal = newData.m_iKor + newData.m_iMat + newData.m_iEng;
	newData.m_fAverage = newData.m_iTotal / 3.0f;
	newData.m_iIndex = g_iNumTotal;
	// ���� �޸� �����͸� ���� ���� �Ѵ�.	
	FileSave();

	// ���� �ű� �����͸� ������ ���� �߰� �����Ѵ�.
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
			printf((c_file.attrib & _A_NORMAL) ? " Y " : " N ");	// �Ϲ�����
			printf((c_file.attrib & _A_RDONLY) ? " Y " : " N ");	// �б�����
			printf((c_file.attrib & _A_SYSTEM) ? " Y " : " N ");	// �ý�������
			printf((c_file.attrib & _A_HIDDEN) ? " Y " : " N ");	// ��������
			printf((c_file.attrib & _A_ARCH) ? " Y " : " N ");		// ��������(������ ���� �� ���ﶧ ����)
			printf((c_file.attrib & _A_SUBDIR) ? " Y " : " N ");	// �������丮

			ctime_s(buffer, _countof(buffer), &c_file.time_write);

			printf(" %-12s %.24s %9ld\n",
				c_file.name, ctime(&(c_file.time_write)), c_file.size);
			
			SortFile(&c_file);
			
		} while (_findnext(hFile, &c_file) == 0);

		_findclose(hFile);
	}
}
// �ֱ� ������ ���ڸ� �������� �տ��� ���� ���ĵȴ�.
void SortFile(_finddata_t* c_file)
{
	// �ִ� 100���� �����Ѵ�.
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