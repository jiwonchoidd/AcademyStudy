#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h> // srand(), rand()
#include <stdio.h> 
#include <time.h>  // time()

void Swap(int *a, int *b)
{
	int Temp;
	Temp = *a;
	*a = *b;
	*b = Temp;
}

void RebuildHeap(int list[], int iSize)
{
	// 레벨오더로 구성되었기 때문에 iLastIndex는 정렬된 노드의 앞 노드가 된다.
	for (int  iLast = iSize - 1; iLast >= 0; iLast--)
	{
		// 마지막 노드에 루트 노드를 대입한다. 이후 자노드 중에서 가장 큰 노드를 루트로 올린다.
		Swap(&list[0], &list[iLast]);

		int iRoot = 0;
		int iChild;

		do{
			// 루트노드의 왼쪽 노드 
			iChild = 2 * iRoot + 1;
			// 자노드 중에 큰 노드를 선택하고
			if (iChild < iLast - 1 && (list[iChild] < list[iChild + 1]) )
			{
				iChild++; // 오른쪽 노드
			}
			// 선택된 자노드 루트의 노드보다 크다면 교환한다.
			if (iChild<iLast && list[iRoot]<list[iChild] )
			{
				Swap(&list[iRoot], &list[iChild]);
			}
			iRoot = iChild;
		} while (iChild < iLast);
	}
}
/* 최대 힙 생성 */
void BuildHeap(int list[], int length)
{
	for (int iNode = 1; iNode < length; iNode++)
	{
	   int iChild = iNode;
	   do{
		   int iRoot = (iChild - 1) / 2;
		   if (list[iRoot] < list[iChild])
		   {
			   Swap(&list[iRoot], &list[iChild]);
		   }
		   iChild = iRoot;
	   } while (iChild != 0);
	}
}

void HeapSort(int list[], int length)
{
	BuildHeap(list, length);
	printf("\n %s", "힙 생성-->");
	for (int i = 0; i<length; i++)
	{
		printf(" %d", list[i]); // 현재의 정렬상태 확인
	}
	RebuildHeap(list, length);
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
#define iMaxCnt 10
void main()
{
	int item[iMaxCnt];
	SetData(item, iMaxCnt);
	DataPrint(item, iMaxCnt);
	
	//Size = sizeof(data) / sizeof(data[0]);
	HeapSort(item, _countof(item));
	printf("\n %s", "정렬 -->");
	DataPrint(item, iMaxCnt);
	getchar();
}
