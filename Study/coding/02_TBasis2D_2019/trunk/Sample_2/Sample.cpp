#include <stdlib.h> // srand(), rand()
#include <stdio.h> 
#include <time.h>  // time()

int Size, Count = 0;
void Swap( int *a, int *b)
{
	 int Temp;
	 Temp = *a;
	 *a = *b;
	 *b = Temp;
}

int Partition(int DataList[], int Begin, int End)
{
	int Temp, Pivot, L, R;
	
	L = Begin+1;
	R = End;
	Pivot = Begin;// (int)((Begin + End) / 2);

	Count++;
	printf("\n [%d�ܰ�] : Pivot = %d",Count, DataList[Pivot]);

	while(L<=R)
	{
		while((DataList[L]<=DataList[Pivot]) && L <= End) L++;
		while((DataList[R]>=DataList[Pivot]) && R >= Begin+1) R--;

		if( L<=R )
		{
			printf("\n [%d, %d] swap(%d, %d)", 
				L, R, DataList[L], DataList[R]);
			Swap(&DataList[L], &DataList[R]);
			for (int i = 0; i<Size; i++)
			{
				printf(" %d", DataList[i]); // ������ ���Ļ��� Ȯ��
			}
		}
	}
	// �ǹ��� �ڸ��� �ִ´�.
	printf("\n [%d, %d] swap(%d, %d)",
		Begin, R, DataList[Begin], DataList[R]);
	Swap(&DataList[Begin], &DataList[R]);

	for(int i=0; i<Size; i++)
	{
		printf(" %d",DataList[i]); // ������ ���Ļ��� Ȯ��
	}
	return R;
}

void QuickSort(int DataList[], int Begin, int End)
{
	if(Begin < End)
	{
		int p = Partition(DataList, Begin, End);
		QuickSort(DataList, Begin, p-1);		
		QuickSort(DataList, p+1, End);		
	}
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
//const int iMaxCnt = 3000;
#define iMaxCnt 10000

void main()
{
	srand( (unsigned)time( NULL ) ); 
	int data[iMaxCnt];
	SetData(data, iMaxCnt);
	DataPrint(data, iMaxCnt);
		
	printf("\n %s", "���� ����-->");
	//int Size = sizeof(data)/sizeof(data[0]);  
	// ���� ���� �����͸� �������� ������ ��͸� ����ϸ� ������ ���ļ� 
	// ������ ���� Ȯ���� ��������. �׷��� ����͸� ����ϴ� ���̴�.
	QuickSort(data, 0, iMaxCnt -1);
	DataPrint(data, iMaxCnt);
}

