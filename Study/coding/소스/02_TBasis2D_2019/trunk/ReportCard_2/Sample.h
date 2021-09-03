#pragma once
#include <cstdio>
#include <cstdlib> // _countof 정적할당 배열 크기 반환
#include <time.h>
#include <string.h>
#include <math.h>
#define BACKWARD_LINK

#define KGCA typedef struct Subject {
#define END }SUB;
// 재정의
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

// 함수 포인터( 함수의 주소를 저장) 정의
// 반환값과 인자값이 동일한 함수의 주소만 저장 할 수 있다.
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