#include <stdlib.h> // srand(), rand()
#include <stdio.h> 
#include <time.h>  // time()
  
#define MAX_NUM 100
int g_iStackBuffer[MAX_NUM];
int g_iTopStack = 0;

int g_iQueueBuffer[MAX_NUM];
int g_iRearQueue = 0;
int g_iFrontQueue = 0;
bool g_bFlagQueue = false;

void Swap( int* a, int* b )
{
	if(*a>*b)
	{
		int t= *a;
		*a = *b;
		*b = t;
 	}
}

//////////////////////////////////////////
// Statk
//////////////////////////////////////////
void InitStack()
{
	g_iTopStack = 0;
} 

void StackPush(int i)
{  
	if( g_iTopStack >= MAX_NUM) 
	{
		printf("g_iStackBuffer full\n");
		return;
	}
	else
	{
		g_iStackBuffer[g_iTopStack++] = i;
	}
}
  
int StackPop()
{  
	if( g_iTopStack == 0)
		return 0;
	else
		return g_iStackBuffer[--g_iTopStack];  
}
  
int IsStatkEmpty()
{  
	if( g_iTopStack == 0)
		return 1;
	else
		return 0;  
}
//////////////////////////////////////////
// Queue
//////////////////////////////////////////
void InitQueue()
{
	g_iRearQueue = 0;
	g_iFrontQueue = 0;
} 
// 0번째 방은 제일 마지막에 넣는다.
void QueuePush(int i)
{  
	if( g_bFlagQueue == true ) 
	{
		printf("g_iQueueBuffer full\n");
		return;
	}
	g_iRearQueue = (g_iRearQueue+1) % MAX_NUM;
	if( g_iFrontQueue == g_iRearQueue) 
	{		
		// 이코드가 없으면 계속 다음칸에 복사하여 넣는다.
		g_bFlagQueue = true;
	}
	g_iQueueBuffer[g_iRearQueue] = i;
}
  
int QueuePop()
{  
	if( g_iFrontQueue == g_iRearQueue && g_bFlagQueue == false )
	{
		printf("\ng_iQueueBuffer Empty");
		return 0;
	}
	g_bFlagQueue = false;
	g_iFrontQueue = (g_iFrontQueue+1) % MAX_NUM;
	return g_iQueueBuffer[g_iFrontQueue];
}
  
int IsQueueEmpty()
{  
	if( g_iFrontQueue == g_iRearQueue)
		return 1;
	else
		return 0;  
}
