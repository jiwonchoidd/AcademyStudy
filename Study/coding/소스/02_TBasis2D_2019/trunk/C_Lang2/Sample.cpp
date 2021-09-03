//#define CPLUSPLUS

#ifndef CPLUSPLUS
	#include <stdio.h> // <cstdio>
	#include <stdlib.h>// <cstdlib> _countof
	#include <ctype.h> // toupper, tolower
	#include <conio.h>
	#include <malloc.h> //_msize 없어도 위에서 연결되어 있다.
#else
	#include <iostream>
	#include <conio.h>
#endif

int Compute2(int iValueX, int iValueY)
{
	int iAdd = iValueX + iValueY;
	iValueX = iAdd;
	return iAdd;
}

int Compute( int *iValueX, int *iValueY )
{
	int iAdd = *iValueX + *iValueY;
	*iValueX = iAdd;
	return iAdd;
}
void ComputeArray( int iList[6], int n )
{
	for( int iCnt = 0; iCnt < n; iCnt++ )
	{
		iList[iCnt] += 3;
	}
}
void ComputeArray2(int* iList, int n)
{
	for (int iCnt = 0; iCnt < n; iCnt++)
	{
		iList[iCnt] += 3;
	}
}
char* StringToUpper( char *strList )
{	
	int iCount = 0;
	// 아래의 주석은 문제가 발생 중간에 공백 있기 때문에 문제가 발생!
	//iCount = sizeof(strList);	
	while( strList[iCount++] != 0 );

	char* pString = (char*)malloc(sizeof(char)*(iCount));
	int iCnt = 0;
	for( iCnt = 0; iCnt < iCount-1; iCnt++ )
	{
		pString[iCnt] = toupper(strList[iCnt] );
		//pString[iCnt] = tolower(strList[iCnt] );		;
	}
	pString[iCount-1] = 0;
	return pString;
}
void main()
{
	int iX = 10;
	int iY = 20;
	int iArray[] = { 10,20,30,40,50,60 };
	int iSize1 = sizeof(iArray);
	int iSize2 = sizeof(iArray) / sizeof(iArray[0]);
	int iCount = _countof(iArray); // (정적크기)배열 크기 얻기(포인터이면 안된다.)
#ifdef CPLUSPLUS
	std::cout << Compute(&iX, &iY ) << std::endl;
	std::cout << iX << iY << std::endl;
#else
	printf("\n iX+iY : %d", Compute2(iX, iY ));
	printf("\n iX : %d , iY : %d\n", iX, iY );
#endif
	
	int iMaxCnt = sizeof(iArray) / sizeof(iArray[0]);
	int iTotalSize = sizeof(iArray);	

	ComputeArray( iArray,  iMaxCnt);

	for( int iCnt= 0; iCnt< iMaxCnt; iCnt++ )
	{
#ifndef CPLUSPLUS
		printf("%5d\n", iArray[iCnt] );
#else
		std::cout << iArray[iCnt] << std::endl;
#endif
	}
	char *p = StringToUpper("Kgca Game Academy");
	int iMallocCount = _msize(p)/sizeof(char); // 동적 할당(힙에) 크기 얻기(배열의 주소는 안된다.)
#ifndef CPLUSPLUS
	printf("%5d\n", iMallocCount);
#else
	std::cout << iMallocCount << std::endl;
#endif
	puts( p );
	free(p);
	_getch();
}