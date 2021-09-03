// ContainingRecord.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <iostream>
//����ü���� Ư�� field�� �ּҸ� ���ؼ� �� field�� �����ϰ� �ִ� ���� ����ü(��ü)�� �ּҸ� ���� �� �ִ� ����� �ִ�.�׶� ���� ��ũ�ΰ� CONTAINING_RECORD ��ũ���̴�.
//�켱 CONTAINING_RECORD ��ũ��ó�� ������ ��ũ�θ� ���� �Ұ��ϸ�, offsetof ��� ��ũ�ΰ� �ִ�.
//�̸� �̿��ϸ� Ư�� ����ü���� �ش� field�� offset�� �� �� �ִ�.
//
//offsetof ��ũ�ο���(unsigned long)(&((type *)0)->field)  ����, 0�� �־��� type�� �������̶�� �����ϵ��� Ÿ��ĳ������ �� �Ŀ�, �� type�� field�� ������ �Ͽ�, type�� ���������� field ������ �Ÿ��� ���ϴ� ���̴�.��, 0���� field���� offset�� �󸶳ĸ� ���ϴ� ���̴�.
//(�ٷ� �� ����, 3���� �����Ǿ� ��ũ�� ������ ���� ���� ���� �����Ͽ� �ο��� ���Դϴ�.)
//
//���� offsetof ��ũ���� ����� �״�� �̿��� ���� CONTAINING_RECORD ��ũ���̴�.
//�� ��ũ�ο����� ����ü ���� Ư�� field�� ���ؼ� ����ü ���ۺ����� offset ���� ���ϰ�,
//�̸� ���� �� ����ü ��ü���� �� field �ּҿ��� offset ����ŭ ���� �ּҷ� �̵��� ���� ����Ͽ�
//����ü ��ü ��ü�� �ּҸ� ��´�.
//
//Ư�� �̴� ����ü �ȿ� ����Ʈ ������ ���ԵǾ� ���� �� �����ϴ�.
//����Ʈ�� �����ϴ� ����ü�� �̸��� �ȴٸ� Ư�� ����Ʈ�� ���� �ּҸ� �־ �� ����Ʈ�� �����ϴ� ���� ����ü ��ü�� �ּҸ� ���� �� �ִ� ���̴�.
//
//�Ʒ� �ڵ�� ���� �����ؼ� �̸� Ȯ���� ���̴�.
//�ڵ� ���� ���ó�� offsetof�� CONTAINING_RECORD ��ũ�� �� �� �� �����Ѵ�.
//������ �����ߵ���(type *)0���� 0�� ���������� �ϱ⿡,
//�ڵ忡��(testStr*)3 ��� �ؼ� 3�� ���������� �� ���� ������ Ȯ���� �� �ִ�.
//���������� CONTAING_RECORD�� offset �κп� offsetof ��ũ�θ� �־ ��ũ�θ� ������ ���� �ִ�.
//
//������ CONTAING_RECORD ��ũ�ο��� offset �κ� �տ�(unsigned long)�� �ٿ��µ�,
//(unsigned long *)��� �Ǿ� �ִ� �ڷᵵ �ô�.
//Visual Studio 6.0���� �׽�Ʈ �غ��� �� �� ����� �� ���� �����µ�,
//(unsigned long *)��� ���� ��� incompatible types �� compile warning�� ���.

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
