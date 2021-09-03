#define _CRT_SECURE_NO_WARNINGS
#define CPLUSPLUS
#undef CPLUSPLUS

#ifndef CPLUSPLUS	
	#include <stdio.h>	// printf
	#include <stdlib.h> // malloc, free
	#include <conio.h>	//_getche(); console I/O	
	void main()
	{
		int iValue		= 5;
		int* pValue		= &iValue;
		int** ppValue	= &pValue;
		printf("%d ", iValue);
		printf("%d ", *pValue);
		printf("%d ", **ppValue);

		//-----------------------PART 1------------------------
		// ������ ���� �Ҵ�, �⺻ void* ���� int*�� ����ȯ
		//-----------------------------------------------------
		printf("\n=====================\n");
		int *pPoint = (int*)malloc(sizeof(int));
		*pPoint = 100;
#ifdef _WIN64
		printf("%016llx���� : %d\n", pPoint + 0, *(pPoint + 0));		
#else
		printf("%016x���� : %d\n", pPoint + 0, *(pPoint + 0));
#endif
		free(pPoint);

		//-----------------------PART 2------------------------
		// ������ ���� �迭 �Ҵ�
		//-----------------------------------------------------
		printf("\n=====================\n");
		int *pPointArray = (int*)malloc(sizeof(int) * 10);
		for (int iCnt = 0; iCnt < 10; iCnt++)
		{
			pPointArray[iCnt] = iCnt * 10;
			printf("\n0x%08x���� : %d", &pPointArray[iCnt], *(pPointArray + iCnt));
		}
		free(pPointArray);

		//-----------------------PART 3------------------------
		// ������ ������ ���� �迭 �Ҵ�
		//-----------------------------------------------------
		printf("\n========���ڸ� �Է��ϰ� ����ġ�ÿ�!=============\n");
		char* pCharPoint = 0;
		int iCount = 0;
		pCharPoint = (char*)malloc(sizeof(char) * 100);
		// ��Ƽ����Ʈ���� _getche()==0�̶�� ���� Ȯ��Ű�� �ԷµǾ��ٴ� ���̴�.
		// ����Ű �Ǵ� home, end,insert���� 0x00E0(224)+code�� ��ȯ�Ѵ�. ���Ű=[0]+[code]
		do {
			int ivalue = _getch(); //_getche();
			if (ivalue != 0 && ivalue != 0x00E0)
			{
				pCharPoint[iCount++] = ivalue;
				printf("*"); // password
			}

		} while (pCharPoint[iCount - 1] != '\r');

		// ���ĺ� �Ǵ� ����Ű�� �Է½� Ȯ��Ű�ڵ�(�ι��� �߰�)�� ��ȯ�ϴ� 
		// ������ �ý��ۿ� ���� �߻��Ѵ�.
		/*do {
				pCharPoint[iCount++] = _getche();
		} while (pCharPoint[iCount - 1] != '\r');*/

		// \n, \r  
		pCharPoint[iCount - 1] = 0;
		printf("\n0x%08x���� : %s", pCharPoint, pCharPoint);
		for (int iCnt = 0; iCnt < iCount; iCnt++)
		{
			printf("\n0x%08X���� : %c", pCharPoint + iCnt, *(pCharPoint + iCnt));
		}
		free(pCharPoint);
		
		//-----------------------PART 4------------------------
		// ������ �迭 ���� �Ҵ�
		//-----------------------------------------------------
		printf("\n=====================\n");
		int *lpPoint[10];
		for (int iCnt = 0; iCnt < 10; iCnt++)
		{
			lpPoint[iCnt] = (int*)malloc(sizeof(int));
			*lpPoint[iCnt] = iCnt * 10;
			printf("\n0x%08X���� : %d", lpPoint + iCnt, *lpPoint[iCnt]);
		}
		for (int iCnt = 0; iCnt < 10; iCnt++)
		{
			free(lpPoint[iCnt]);
		}
		printf("\n=====================\n");

		//-----------------------PART 5------------------------
		// 2���� �迭 �ʱ�ȭ �� ����
		//-----------------------------------------------------		
		int iValue[3][2] = { { 10,20 },{ 30,40 },{ 50,60 } };
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			// ī����, �ּ�, ��
			printf("\n*(iValue+%d): %p=%d", iCnt, *(iValue + iCnt), *(*(iValue+iCnt)+0));
			printf("\t\tiValue[%d]: %p", iCnt, iValue[iCnt]);
		}
		printf("\n");

		int* pData = iValue[1];	
		*pData++ = *pData + 100; // 130
		*pData++ = *pData + 100; // 140;
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			printf("\n*(iValue+%d): %p", iCnt, *(iValue + iCnt));
			printf("\t\tiValue[%d]: %p", iCnt, iValue[iCnt]);
		}
		printf("\n");

		pData = iValue[0];
		int** ppData = &pData;
		ppData[0][0] = *(*(ppData+0)+0) + 200; // 210
		ppData[0][1] = *(*(ppData+0)+1) + 200; // 220
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			printf("\n*(iValue+%d): %p", iCnt, *(iValue + iCnt));
			printf("\t\tiValue[%d]: %p", iCnt, iValue[iCnt]);
		}
		printf("\n");

		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			for (int jCnt = 0; jCnt < 2; jCnt++)
			{
				printf("\n*(*(iValue+%d)+%d)  : %d", iCnt, jCnt, *(*(iValue + iCnt) + jCnt));
				printf("\t\t *(iValue[%d]+%d) : %d", iCnt, jCnt, *(iValue[iCnt] + jCnt));
			}
		}

		//-----------------------PART 6------------------------
		// ���� ������ �Ҵ� 
		//-----------------------------------------------------		
		printf("\n=====================\n");
		printf("\n====== ���������� =========\n");
		system("cls");
		int *pPtr;
		int **ppPtr;
		int num = 10;
		pPtr = &num;
		//�������� �޸� �ּҴ� �Ϲ�����Ʈ�� ���� �Ұ��ϸ� 
		//�ݵ�� ���������Ϳ� �����ؾ� �Ѵ�.
		ppPtr = &pPtr;//pPtr�� �ּ� ����
		printf("%d\n", **ppPtr);//ppPtr�� �޸� �ּҿ� ����

		//-----------------------PART 7------------------------
		// ��������Ʈ�� �Ҵ��� �ý��� ��Ʈ�� ����ؾ� �Ѵ�.
		//-----------------------------------------------------		
		unsigned char *array[50];
		unsigned int size = sizeof(array);
		//32 - bit  : sizeof(array) = 200
		//64 - bit  : sizeof(array) = 400
#ifdef _WIN64		
		printf("\n%d", size);
		printf("\n64 - bit Size=%d", sizeof(array));
#else
		printf("\n32 - bit Size=%d", sizeof(array));
#endif	
		//-----------------------PART 8------------------------
		// 8����Ʈ ������ �迭 ? 4����Ʈ ������ �迭 ?
		// intptr_t ����ϸ� �ý��� ��Ʈ���� ���� ��ȯ��.
		//-----------------------------------------------------		
		int  **ppPoint = 0;
		ppPoint = (int **)malloc(sizeof(intptr_t) * 3);
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			ppPoint[iCnt] = (int *)malloc(sizeof(int) * 3);
		}

		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			for (int jCnt = 0; jCnt < 3; jCnt++)
			{
				ppPoint[iCnt][jCnt] = iCnt * 3 + jCnt;
			}
		}

		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			printf("\n");
			for (int jCnt = 0; jCnt < 3; jCnt++)
			{
				printf("[%d,%d]=%d:%d", iCnt, jCnt, ppPoint[iCnt][jCnt], *(ppPoint[iCnt] + jCnt));
			}
		}

		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			free(ppPoint[iCnt]);
		}
		free(ppPoint);

		//-----------------------PART 9------------------------
		// ������ �迭
		//-----------------------------------------------------		
		printf("\n====== ������ �迭=========\n");
		system("cls");

		char *pArray[6];
		for (int iCnt = 0; iCnt < 6; iCnt++)
		{
			pArray[iCnt] = (char*)malloc(sizeof(char) * 4);
		}

		for (int iCnt = 0; iCnt < 6; iCnt++)
		{
			for (int jCnt = 0; jCnt < 3; jCnt++)
			{
				pArray[iCnt][jCnt] = 65 + iCnt * 3 + jCnt;
			}		
			// ���ڿ� �������� NULL�� �Ѵ�.
			pArray[iCnt][3] = 0;
		}

		for (int iCnt = 0; iCnt < 6; iCnt++)
		{
			printf("\n");
			printf("[%d]=%s", iCnt, pArray[iCnt]);
		}

		for (int iCnt = 0; iCnt < 6; iCnt++)
		{
			free(pArray[iCnt]);
		}
		_getch();
	}	
#else
	#include <cstdio>
	#include <cstdlib>
	#include <conio.h>
	#include <iostream>

	void main()
	{
		//-----------------------PART 1------------------------
		// ������ ���� �Ҵ�, �⺻ void* ���� int*�� ����ȯ
		//-----------------------------------------------------
		std::cout << "=====================" << std::endl;
		int *pPoint = new int;
		*pPoint = 100;
		std::cout << std::hex  << pPoint+0 << "����:"<< std::dec << *(pPoint + 0) << std::endl;
		delete  pPoint;
		pPoint = 0; //nullptr
		
		//-----------------------PART 2------------------------
		// ������ ���� �迭 �Ҵ�
		//-----------------------------------------------------
		std::cout << "=====================" << std::endl;
		int *pPointArray = new int[10];
		for( int iCnt = 0; iCnt < 10; iCnt++ )
		{		
			pPointArray[iCnt] = iCnt*10;
			std::cout << pPointArray[iCnt] <<"   "<< *(pPointArray+iCnt) <<std::endl;
		}
		delete [] pPointArray;

		//-----------------------PART 3------------------------
		// ������ ������ ���� �迭 �Ҵ�
		//-----------------------------------------------------
		std::cout << "\n=======���ڸ� �Է��ϰ� ����ġ�ÿ�!==============\n";
		char* pCharPoint = 0;
		int iCount = 0;
		pCharPoint = new char[100];
		do{
			pCharPoint[iCount++] = _getche();						
		}while(pCharPoint[iCount-1] != '\r');
	
		pCharPoint[iCount+1] = 0;
		printf("\n0x%08x���� : %s", pCharPoint, pCharPoint );	
		for( int iCnt = 0; iCnt < iCount; iCnt++ )
		{
			printf("\n0x%08X���� : %c", pCharPoint+iCnt, *(pCharPoint+iCnt) );	
		}
		delete[] pCharPoint;
		//-----------------------PART 5------------------------
		// 2���� �迭 �ʱ�ȭ �� ����
		//-----------------------------------------------------	
		std::cout << "=====================" << std::endl;
		int iValue[3][2] = { {10,20}, {30,40}, {50,60} };
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			printf("\n*(iValue+%d): %p", iCnt, *(iValue + iCnt));
			printf("\t\tiValue[%d]: %p", iCnt, iValue[iCnt]);
		}
		std::cout << std::endl;
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			for (int jCnt = 0; jCnt < 2; jCnt++)
			{
				printf("\n*(*(iValue+%d)+%d)  : %d", iCnt, jCnt, *(*(iValue + iCnt) + jCnt));
				printf("\t\t *(iValue[%d]+%d) : %d", iCnt, jCnt, *(iValue[iCnt] + jCnt));
			}
		}
		//-----------------------PART 8------------------------
		// ���������� �Ҵ�
		//-----------------------------------------------------		
		int  **ppPoint = 0;
		ppPoint = new int*[3];
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			ppPoint[iCnt] = new int[3];
		}

		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			for (int jCnt = 0; jCnt < 3; jCnt++)
			{
				ppPoint[iCnt][jCnt] = iCnt * 3 + jCnt;
			}
		}

		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			std::cout << std::endl;
			for (int jCnt = 0; jCnt < 3; jCnt++)
			{
				std::cout << "[" << iCnt << jCnt << "]=" << ppPoint[iCnt][jCnt] <<":"<< *(ppPoint[iCnt] + jCnt);
			}
		}
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			delete [] ppPoint[iCnt];
		}
		delete [] ppPoint;

		//-----------------------PART 9------------------------
		// ������ �迭
		//-----------------------------------------------------	
		std::cout << "=====================" << std::endl;
		int *lpPoint[10];
		for( int iCnt = 0; iCnt < 10; iCnt++ )
		{
			lpPoint[iCnt] = new int;
			*lpPoint[iCnt] = iCnt*10;
			printf("\n0x%08X���� : %d", lpPoint+iCnt, *lpPoint[iCnt] );	
		}
		for( int iCnt = 0; iCnt < 10; iCnt++ )
		{
			delete lpPoint[iCnt];
		}
		
		std::cout << "�ƹ�Ű�� �Է��ϸ� ����˴ϴ�."<< std::endl;
		_getch();	
	}
#endif

