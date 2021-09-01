#include "Sample.h"

void qsort(int a[], int n)
{
	int p, t;
	int i, j;
	int l, r;
  
	InitStack();   
  
	l = 0;
	r = n - 1;
  
	StackPush(r);
	StackPush(l);
  
	while(!IsStatkEmpty())
	{
		l = StackPop();
		r = StackPop();
  
		if( r-l+1 > 1) 
		{  // 종료조건 :: 남아 있는 분할의 원소 개수가 1개 이상일 경우
			p = a[r];
			i = l - 1;
			j = r;
  
			while(1) 
			{
				while(a[++i] < p);
				while(a[--j] > p);  
				if( i >= j)		break;                
				Swap( &a[i], &a[j] );			
			}
  
			// pivot과 i값을 교환
			Swap( &a[i], &a[r] );
  
			StackPush(r);     // 오른쪽 분할의 오른쪽 끝
			StackPush(i+1);  // 오른쪽 분할의 왼쪽 끝
			StackPush(i-1);  // 왼쪽 분할의 오른쪽 끝
			StackPush(l);     // 왼쪽 분할의 왼쪽 끝 
		} // if
	} // while
}
void DataPrint(int item[], int iCount)
{
	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		printf(" %d", item[iCnt]);
	}
}

void SetData(int item[], int iCount)
{
	srand((unsigned int)time(NULL));
	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		item[iCnt] = rand() % iCount;
	}
}
void main()
{
	srand( (unsigned)time( NULL ) ); 
	int data[MAX_NUM];
	for( int i = 0; i < MAX_NUM;i++ )
	{
		data[i] = rand()% MAX_NUM;
		StackPush( data[i] );
		QueuePush( data[i] );
	}
	// 원시 데이터
	printf("\n원시 데이터");
	DataPrint(data, MAX_NUM);

	//int Size = sizeof(data)/sizeof(data[0]);  
	printf("\n스택 데이터");
	for(int i=0; i<MAX_NUM; i++)
	{
		printf(" %d",StackPop()); 
	}
	//printf("%c",'\n');
	printf("\n큐 데이터");
	for(int i=0; i<MAX_NUM; i++)
	{
		printf(" %d",QueuePop()); 
	}
	printf("%c",'\n');
	
	qsort(data, MAX_NUM);

	printf("\n퀵 정렬 데이터");
	DataPrint(data, MAX_NUM);
}

