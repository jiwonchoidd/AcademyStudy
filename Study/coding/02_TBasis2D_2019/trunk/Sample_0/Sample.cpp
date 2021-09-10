#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>

int compare(const void *a, const void *b)    // �������� �� �Լ� ����
{
	int num1 = *(int *)a;    
	int num2 = *(int *)b;    
	if (num1 < num2)    // a�� b���� ���� ����
		return -1;      // -1 ��ȯ
	if (num1 > num2)    // a�� b���� Ŭ ����
		return 1;       // 1 ��ȯ
	return 0;    // a�� b�� ���� ���� 0 ��ȯ
}

bool SwapGreat( int* a, int* b )
{
	if(*a>*b)
	{
		int t= *a;
		*a = *b;
		*b = t;
		return true;
 	}
	return false;
}
void Change(int *a, int *b)
{
	int Temp;
	Temp = *a;
	*a = *b;
	*b = Temp;
}
void SelectionSort(int item[],int count)
{
	int iIndex;
	for (int a = 0; a < count - 1; a++)
	{
		iIndex = a;
		for (int b = a + 1; b < count; b++)
		{
			if (item[b] < item[iIndex])
			{
				iIndex = b;
			}
		}
		if (a != iIndex)
		{
			SwapGreat(&item[a], &item[iIndex]);
		}
	}
}
void BubbleSort(int item[], int count)
{
	for (int a = 0; a<count-1; a++)
	{
		bool bFlag = false;
		for (int b = 0; b <  count - a - 1; b++ )
		{
			if (SwapGreat(&item[b], &item[b + 1]))
			{
				bFlag = true;
			}
		}
		if (bFlag == false) break;
	}
}
void InsertionSort(int item[], int count, int iStart=1, int iStep=1)
{
	for (int a = iStart; a<count; a+= iStep )
	{
		int temp = item[a];
		int b = a - iStep;
		while (b >= 0 && item[b] > temp)
		{
			item[b + iStep] = item[b];
			b-= iStep;
		}
		item[b + iStep] = temp;
	}
}
// �������� �� ���ĺ��� 2~3�� ������. ������ �����ĵ� ���� ���� ������ ������.
// �� ������ �޸� �ѵ������� �󸶵��� ������ �� ������
// �� ������ ������ ��������ν� ���³�ħ(Stack overflow)�� ����� �����ϰ� �ִ�.
void shellsort(int item[], int n)
{
	// n�� ���� ���� �ִ� �׷��� ������ ã�´�.
	// 1, 2, 3,  4,  5
	// 1, 4, 13, 40, 121, ...
	int maxGroup = 1;
	for (int k = 1; k < n; k++)
	{
		int g = (pow(3, k) - 1) / 2; 
		if (g > n)
		{
			break;
		}
		maxGroup = k;
	};

	while (maxGroup > 0)
	{
		int g = (pow(3, maxGroup--) - 1) / 2;
		// �κ����� ����
		for (int s = 0; s < g; s++)
		{
			InsertionSort(item, n, s, g);
		}
	}
}
void DataPrint(int item[], int iCount)
{	
	return;
	for (int t = 0; t<iCount; t++)
	{
		if (t % 20 == 0)
		{
			printf("\n");
		}
		printf(" %d", item[t]);
	}
}
void Merge(int item[],  int iStart, int iEnd, int iMiddle)
{
	std::vector<int> itemList;
	int i = iStart;
	int j = iMiddle + 1;
	// �� �׷��� �����͸� ���Ͽ� ���� ���� i������
	// ū���� j�׷쿡 �ִ´�.
	while (i <= iMiddle && j <= iEnd)
	{
		if (item[i] < item[j])
		{
			itemList.push_back(item[i++]);
		}
		else //if(item[i] > item[j])
		{
			itemList.push_back(item[j++]);
		}
	}
	// �α׷��� �������� ü�� �ִ´�.
	while( i<=iMiddle) itemList.push_back(item[i++]);
	while (j <= iEnd) itemList.push_back(item[j++]);

	int copy = 0;
	for (int k = iStart; k <= iEnd; k++)
	{
		item[k] = itemList[copy++];
	}
}
void MergeSort(int item[],  int iStart, int iEnd)
{
	if (iStart < iEnd)
	{
		int iMiddle = (iStart + iEnd) / 2;
		// ����(Divide)
		MergeSort(item,  iStart, iMiddle);
		MergeSort(item,  iMiddle+1, iEnd);
		// �պ�(Conquer)
		Merge(item, iStart, iEnd, iMiddle);
	}
}
void QuickSort(int item[], int iStart, int iEnd)
{
	int iPivot = item[iStart];
	int iBeginStart = iStart;
	int iBeginEnd = iEnd;
	while (iStart < iEnd)
	{
		while (iPivot <= item[iEnd] && iStart < iEnd)
		{
			iEnd--;
		}
		if (iStart > iEnd) break;
		while (iPivot >= item[iStart] && iStart < iEnd)
		{
			iStart++;
		}
		if (iStart > iEnd) break;
		SwapGreat(&item[iStart], &item[iEnd]);
	}
	SwapGreat(&item[iBeginStart], &item[iStart]);

	if (iBeginStart < iStart)
		QuickSort(item, iBeginStart, iStart - 1);
	if (iBeginEnd > iEnd)
		QuickSort(item, iStart + 1, iBeginEnd);
}
void RebuildHeap(int list[], int iSize)
{
	// ���������� �����Ǿ��� ������ iLastIndex�� ���ĵ� ����� �� ��尡 �ȴ�.
	for (int iLast = iSize - 1; iLast >= 0; iLast--)
	{
		// ������ ��忡 ��Ʈ ��带 �����Ѵ�. ���� �ڳ�� �߿��� ���� ū ��带 ��Ʈ�� �ø���.
		Change(&list[0], &list[iLast]);

		int iRoot = 0;
		int iChild;

		do {
			// ��Ʈ����� ���� ��� 
			iChild = 2 * iRoot + 1;
			// �ڳ�� �߿� ū ��带 �����ϰ�
			if (iChild < iLast - 1 && (list[iChild] < list[iChild + 1]))
			{
				iChild++; // ������ ���
			}
			// ���õ� �ڳ�� ��Ʈ�� ��庸�� ũ�ٸ� ��ȯ�Ѵ�.
			if (iChild<iLast && list[iRoot]<list[iChild])
			{
				Change(&list[iRoot], &list[iChild]);
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
		do {
			int iRoot = (iChild - 1) / 2;
			if (list[iRoot] < list[iChild])
			{
				Change(&list[iRoot], &list[iChild]);
			}
			iChild = iRoot;
		} while (iChild != 0);
	}
}

void HeapSort(int list[], int length)
{
	BuildHeap(list, length);
	RebuildHeap(list, length);
}
void SetData(int item[], int iCount)
{
	// �ݺ������� ���� �迭 ���� �������� ó���Ѵ�.
	srand((unsigned int)time(NULL));

	for (int a = 0; a<iCount; a++)
	{
		item[a] = rand() % iCount;
	}
	DataPrint(item, iCount);
}
void main()
{
	int item[100000];
	int count = 10000;

	clock_t sTime, eTime;
	printf("How many numbers(MAX=10000? ");
	scanf("  %d", &count);
	getchar();	

	printf("\nSelect Sort ");
	SetData(item, count);
	sTime = clock();	
	SelectionSort(item, count);
	eTime = clock() - sTime;
	printf("\n\ttime => [%ld,%8.3f] => ", eTime, eTime / (double)CLK_TCK);
	DataPrint(item, count);

	printf("\nBubble Sort ");
	SetData(item, count);
	sTime = clock();
	BubbleSort(item, count);
	eTime = clock() - sTime;
	printf("\n\ttime => [%ld,%8.3f] => ", eTime, eTime / (double)CLK_TCK);
	DataPrint(item, count);

	printf("\nInsert Sort ");
	SetData(item, count);
	sTime = clock();
	InsertionSort(item, count);
	eTime = clock() - sTime;
	printf("\n\ttime =>[%ld,%8.3f] => ", eTime, eTime / (double)CLK_TCK);
	DataPrint(item, count);

	printf("\nshell Sort ");
	SetData(item, count);
	sTime = clock();
	shellsort(item, count);
	eTime = clock() - sTime;
	printf("\n\ttime =>[%ld,%8.3f] => ", eTime, eTime / (double)CLK_TCK);
	DataPrint(item, count);

	printf("\nMerge Sort ");
	SetData(item, count);

	sTime = clock();
	MergeSort(item, 0, count - 1);
	eTime = clock() - sTime;
	printf("\n\ttime =>[%ld,%8.3f] => ", eTime, eTime / (double)CLK_TCK);
	DataPrint(item, count);
	
	printf("\nHeap Sort ");
	SetData(item, count);
	sTime = clock();
	HeapSort(item, count);
	eTime = clock() - sTime;
	printf("\n\ttime =>[%ld,%8.3f] => ", eTime, eTime / (double)CLK_TCK);
	DataPrint(item, count);


	printf("\nQuick Sort ");
	SetData(item, count);
	sTime = clock();
	QuickSort(item, 0, count - 1);
	eTime = clock() - sTime;
	printf("\n\ttime =>[%ld,%8.3f] => ", eTime, eTime / (double)CLK_TCK);
	DataPrint(item, count);

	printf("\nQuick Sort ");
	SetData(item, count);
	sTime = clock();
	qsort(item, count, sizeof(int), compare);

	eTime = clock() - sTime;
	printf("\n\ttime =>[%ld,%8.3f] => ", eTime, eTime / (double)CLK_TCK);
	DataPrint(item, count);

	getchar();
}
