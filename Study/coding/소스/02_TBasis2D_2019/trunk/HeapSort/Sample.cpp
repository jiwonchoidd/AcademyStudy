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
	// ���������� �����Ǿ��� ������ iLastIndex�� ���ĵ� ����� �� ��尡 �ȴ�.
	for (int  iLast = iSize - 1; iLast >= 0; iLast--)
	{
		// ������ ��忡 ��Ʈ ��带 �����Ѵ�. ���� �ڳ�� �߿��� ���� ū ��带 ��Ʈ�� �ø���.
		Swap(&list[0], &list[iLast]);

		int iRoot = 0;
		int iChild;

		do{
			// ��Ʈ����� ���� ��� 
			iChild = 2 * iRoot + 1;
			// �ڳ�� �߿� ū ��带 �����ϰ�
			if (iChild < iLast - 1 && (list[iChild] < list[iChild + 1]) )
			{
				iChild++; // ������ ���
			}
			// ���õ� �ڳ�� ��Ʈ�� ��庸�� ũ�ٸ� ��ȯ�Ѵ�.
			if (iChild<iLast && list[iRoot]<list[iChild] )
			{
				Swap(&list[iRoot], &list[iChild]);
			}
			iRoot = iChild;
		} while (iChild < iLast);
	}
}
/* �ִ� �� ���� */
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
	printf("\n %s", "�� ����-->");
	for (int i = 0; i<length; i++)
	{
		printf(" %d", list[i]); // ������ ���Ļ��� Ȯ��
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
	printf("\n %s", "���� -->");
	DataPrint(item, iMaxCnt);
	getchar();
}
