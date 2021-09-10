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
		// 포인터 변수 할당, 기본 void* 에서 int*로 형변환
		//-----------------------------------------------------
		printf("\n=====================\n");
		int *pPoint = (int*)malloc(sizeof(int));
		*pPoint = 100;
#ifdef _WIN64
		printf("%016llx번지 : %d\n", pPoint + 0, *(pPoint + 0));		
#else
		printf("%016x번지 : %d\n", pPoint + 0, *(pPoint + 0));
#endif
		free(pPoint);

		//-----------------------PART 2------------------------
		// 포인터 변수 배열 할당
		//-----------------------------------------------------
		printf("\n=====================\n");
		int *pPointArray = (int*)malloc(sizeof(int) * 10);
		for (int iCnt = 0; iCnt < 10; iCnt++)
		{
			pPointArray[iCnt] = iCnt * 10;
			printf("\n0x%08x번지 : %d", &pPointArray[iCnt], *(pPointArray + iCnt));
		}
		free(pPointArray);

		//-----------------------PART 3------------------------
		// 문자형 포인터 변수 배열 할당
		//-----------------------------------------------------
		printf("\n========문자를 입력하고 엔터치시오!=============\n");
		char* pCharPoint = 0;
		int iCount = 0;
		pCharPoint = (char*)malloc(sizeof(char) * 100);
		// 멀티바이트에서 _getche()==0이라는 것은 확장키가 입력되었다는 것이다.
		// 방향키 또는 home, end,insert등은 0x00E0(224)+code을 반환한다. 펑션키=[0]+[code]
		do {
			int ivalue = _getch(); //_getche();
			if (ivalue != 0 && ivalue != 0x00E0)
			{
				pCharPoint[iCount++] = ivalue;
				printf("*"); // password
			}

		} while (pCharPoint[iCount - 1] != '\r');

		// 알파벳 또는 숫자키를 입력시 확장키코드(널문자 추가)로 반환하는 
		// 문제가 시스템에 따라 발생한다.
		/*do {
				pCharPoint[iCount++] = _getche();
		} while (pCharPoint[iCount - 1] != '\r');*/

		// \n, \r  
		pCharPoint[iCount - 1] = 0;
		printf("\n0x%08x번지 : %s", pCharPoint, pCharPoint);
		for (int iCnt = 0; iCnt < iCount; iCnt++)
		{
			printf("\n0x%08X번지 : %c", pCharPoint + iCnt, *(pCharPoint + iCnt));
		}
		free(pCharPoint);
		
		//-----------------------PART 4------------------------
		// 포인터 배열 변수 할당
		//-----------------------------------------------------
		printf("\n=====================\n");
		int *lpPoint[10];
		for (int iCnt = 0; iCnt < 10; iCnt++)
		{
			lpPoint[iCnt] = (int*)malloc(sizeof(int));
			*lpPoint[iCnt] = iCnt * 10;
			printf("\n0x%08X번지 : %d", lpPoint + iCnt, *lpPoint[iCnt]);
		}
		for (int iCnt = 0; iCnt < 10; iCnt++)
		{
			free(lpPoint[iCnt]);
		}
		printf("\n=====================\n");

		//-----------------------PART 5------------------------
		// 2차원 배열 초기화 및 접근
		//-----------------------------------------------------		
		int iValue[3][2] = { { 10,20 },{ 30,40 },{ 50,60 } };
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			// 카운터, 주소, 값
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
		// 이중 포인터 할당 
		//-----------------------------------------------------		
		printf("\n=====================\n");
		printf("\n====== 이중포인터 =========\n");
		system("cls");
		int *pPtr;
		int **ppPtr;
		int num = 10;
		pPtr = &num;
		//포인터의 메모리 주소는 일반포인트에 저장 불가하며 
		//반드시 이중포인터에 저장해야 한다.
		ppPtr = &pPtr;//pPtr의 주소 저장
		printf("%d\n", **ppPtr);//ppPtr의 메모리 주소에 접근

		//-----------------------PART 7------------------------
		// 이중포인트의 할당은 시스템 비트를 고려해야 한다.
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
		// 8바이트 포인터 배열 ? 4바이트 포인터 배열 ?
		// intptr_t 사용하면 시스템 비트수에 따라 변환됨.
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
		// 포인터 배열
		//-----------------------------------------------------		
		printf("\n====== 포인터 배열=========\n");
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
			// 문자열 마지막은 NULL로 한다.
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
		// 포인터 변수 할당, 기본 void* 에서 int*로 형변환
		//-----------------------------------------------------
		std::cout << "=====================" << std::endl;
		int *pPoint = new int;
		*pPoint = 100;
		std::cout << std::hex  << pPoint+0 << "번지:"<< std::dec << *(pPoint + 0) << std::endl;
		delete  pPoint;
		pPoint = 0; //nullptr
		
		//-----------------------PART 2------------------------
		// 포인터 변수 배열 할당
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
		// 문자형 포인터 변수 배열 할당
		//-----------------------------------------------------
		std::cout << "\n=======문자를 입력하고 엔터치시오!==============\n";
		char* pCharPoint = 0;
		int iCount = 0;
		pCharPoint = new char[100];
		do{
			pCharPoint[iCount++] = _getche();						
		}while(pCharPoint[iCount-1] != '\r');
	
		pCharPoint[iCount+1] = 0;
		printf("\n0x%08x번지 : %s", pCharPoint, pCharPoint );	
		for( int iCnt = 0; iCnt < iCount; iCnt++ )
		{
			printf("\n0x%08X번지 : %c", pCharPoint+iCnt, *(pCharPoint+iCnt) );	
		}
		delete[] pCharPoint;
		//-----------------------PART 5------------------------
		// 2차원 배열 초기화 및 접근
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
		// 이중포인터 할당
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
		// 포인터 배열
		//-----------------------------------------------------	
		std::cout << "=====================" << std::endl;
		int *lpPoint[10];
		for( int iCnt = 0; iCnt < 10; iCnt++ )
		{
			lpPoint[iCnt] = new int;
			*lpPoint[iCnt] = iCnt*10;
			printf("\n0x%08X번지 : %d", lpPoint+iCnt, *lpPoint[iCnt] );	
		}
		for( int iCnt = 0; iCnt < 10; iCnt++ )
		{
			delete lpPoint[iCnt];
		}
		
		std::cout << "아무키나 입력하면 종료됩니다."<< std::endl;
		_getch();	
	}
#endif

