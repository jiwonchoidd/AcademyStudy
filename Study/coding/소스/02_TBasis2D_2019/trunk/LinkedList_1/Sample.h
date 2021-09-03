#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // rand(), srand()
#include <conio.h>
#include <time.h> // time()
typedef struct TStudentNode
{
	char	m_szName[20];
	int		m_iAge;
	int     m_iKor;
	int     m_iEng;
	int     m_iMat;
	int     m_iTotal;
	float   m_fAverage;
	char	m_strAddress[50];
	TStudentNode*  pNext;
}TNode;

TNode*  g_pNodeHead = 0;

int			g_iNumCounter = 0;
//  �ű� ��� ���� �� ���Ḯ��Ʈ ����
void		NewLink(char* pName, int m_iAge, char* pAddr, int* pData = 0);
// �ʱ� ���Ḯ��Ʈ ����
void		RandData();
// pDelNode�� ���Ḯ��Ʈ���� ����
TNode*		DelLink(TNode* pDelNode);
// ��ü ���Ḯ��Ʈ ����
void		AllDeleteLink();
// ���Ϸ� ���� �ε� �� ���Ḯ��Ʈ ����
void		LoadLink(char* pFileName);
// ���Ḯ��Ʈ ��ü�� ���Ϸ� ����.
void		SaveData(char* pSaveFileName);

