// ContainingRecord.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
//구조체안의 특정 field의 주소를 통해서 그 field를 포함하고 있는 실제 구조체(객체)의 주소를 얻을 수 있는 방법이 있다.그때 쓰는 매크로가 CONTAINING_RECORD 매크로이다.
//우선 CONTAINING_RECORD 매크로처럼 유용한 매크로를 먼저 소개하면, offsetof 라는 매크로가 있다.
//이를 이용하면 특정 구조체에서 해당 field의 offset을 알 수 있다.
//
//offsetof 매크로에서(unsigned long)(&((type *)0)->field)  뜻은, 0을 주어진 type의 시작점이라고 가정하도록 타입캐스팅을 한 후에, 그 type의 field를 엑세스 하여, type의 시작점부터 field 까지의 거리를 구하는 것이다.즉, 0부터 field까지 offset이 얼마냐를 구하는 것이다.
//(바로 위 문단, 3줄은 데브피아 링크의 김희준 님의 글을 조금 수정하여 인용한 것입니다.)
//
//위의 offsetof 매크로의 기능을 그대로 이용한 것이 CONTAINING_RECORD 매크로이다.
//이 매크로에서는 구조체 안의 특정 field에 대해서 구조체 시작부터의 offset 값을 구하고,
//이를 실제 그 구조체 객체에서 그 field 주소에서 offset 값만큼 작은 주소로 이동한 값을 계산하여
//구조체 객체 자체의 주소를 얻는다.
//
//특히 이는 구조체 안에 리스트 구조가 포함되어 있을 때 유용하다.
//리스트를 포함하는 구조체의 이름만 안다면 특정 리스트의 시작 주소를 넣어서 그 리스트를 포함하는 실제 구조체 객체의 주소를 얻을 수 있는 것이다.
//
//아래 코드는 직접 실행해서 이를 확인한 것이다.
//코드 실행 결과처럼 offsetof와 CONTAINING_RECORD 매크로 둘 다 잘 동작한다.
//위에서 설명했듯이(type *)0에서 0을 시작점으로 하기에,
//코드에서(testStr*)3 라고 해서 3을 시작점으로 할 수도 있음을 확인할 수 있다.
//참고적으로 CONTAING_RECORD의 offset 부분에 offsetof 매크로를 넣어서 매크로를 구성할 수도 있다.
//
//끝으로 CONTAING_RECORD 매크로에서 offset 부분 앞에(unsigned long)을 붙였는데,
//(unsigned long *)라고 되어 있는 자료도 봤다.
//Visual Studio 6.0에서 테스트 해보니 둘 다 제대로 된 값은 나오는데,
//(unsigned long *)라고 했을 경우 incompatible types 의 compile warning이 뜬다.

#define offsetof(type, member)   (size_t)&(((type *)0)->member)
#define CONTAINING_RECORD(address, type, field) \
		((type *)((char *)(address) - (unsigned long)(&((type *)0)->field)))

typedef struct _testStr 
{
	int a;
	char b[6];
	char c;
}testStr, *ptrTestStr;

int main()
{
	ptrTestStr ptr1 = (ptrTestStr)malloc(sizeof(testStr));
	char * bPtr = ptr1->b;

	printf("ptr1		: [%x]\n", ptr1);
	printf("&ptr1->c	: [%x]\n", &ptr1->c);
	printf("offsetof(testStr, c) : [%d]\n\n", offsetof(testStr, c));

	printf("(testStr*)0	: [%d]\n", (testStr*)0);
	printf("(testStr*)3	: [%d]\n", (testStr*)3);
	printf("(testStr*)3->c	: [%d]\n\n", &((testStr*)3)->c);

	printf("bPtr : [%x]\n", bPtr);
	printf("CONTAINING_RECORD(bPtr, testStr, b) : [%x]\n\n", \
		CONTAINING_RECORD(bPtr, testStr, b));

	return 0;
}
