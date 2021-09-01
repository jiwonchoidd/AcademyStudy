#pragma once
#include <cstdio>
#include <cstdlib> // _countof �����Ҵ� �迭 ũ�� ��ȯ
#include <time.h>
#include <string.h>
#include <math.h>
#define BACKWARD_LINK

#define KGCA typedef struct Subject {
#define END }SUB;
// ������
typedef int SUBJECT;

typedef struct TStudentNode
{
	char m_szName[4];
	int  m_iKor;
	int  m_iEng;
	int  m_iMat;
	TStudentNode* pPrev;
	TStudentNode* pNext;
}TNode;

TNode*  g_pNodeHead = 0;
TNode*  g_pNodeCurrent = 0;
TNode*  g_pNodeTail = 0;

enum STATE {
	DATA_CREATE = 0,
	DATA_LOAD,
	DATA_PRINT,	
	DATA_FIND,
	DATA_FRONT_INSERT,
	DATA_BACK_INSERT,
};

// �Լ� ������( �Լ��� �ּҸ� ����) ����
// ��ȯ���� ���ڰ��� ������ �Լ��� �ּҸ� ���� �� �� �ִ�.
bool(*PROCESS) ();
//PROCESS()
bool Execute()
{
	return PROCESS();
}

bool Load()
{
	printf("load");
	return true;
}
// PROCESS = Load;
// PROCESS()  -> Load()
// PROCESS = Save;
// PROCESS()  -> Save()