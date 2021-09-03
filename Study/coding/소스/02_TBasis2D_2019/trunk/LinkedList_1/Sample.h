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
//  신규 노드 생성 및 연결리스트 연결
void		NewLink(char* pName, int m_iAge, char* pAddr, int* pData = 0);
// 초기 연결리스트 구축
void		RandData();
// pDelNode를 연결리스트에서 삭제
TNode*		DelLink(TNode* pDelNode);
// 전체 연결리스트 삭제
void		AllDeleteLink();
// 파일로 부터 로드 및 연결리스트 구축
void		LoadLink(char* pFileName);
// 연결리스트 전체를 파일로 저장.
void		SaveData(char* pSaveFileName);

