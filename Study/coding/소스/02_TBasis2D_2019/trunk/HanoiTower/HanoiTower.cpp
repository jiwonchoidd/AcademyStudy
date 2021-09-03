//////////////////////  재귀호출/////////////////////////
// 사용자 정의 함수사용
// if()
// 재귀호출
//////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int iCount = 0;
///////////////////////////////////////////////////////////////////
// 1,디스크는 한번에 하나씩 옮겨야 한다.
// 2,위에 놓인 디스크는 아래쪽의 디스크보다 클 수 없다.
//  n
// 2  - 1  번 이동한다.
///////////////////////////////////////////////////////////////////
void HanioTab(int n, char from, char temp, char to)
{
	if (n > 0)
	{
		//a를 c을 임시저장소로 활용하여 b로 놓는다.
		HanioTab(n - 1, from, to, temp);
		printf("[%d]번 %c -> %c\n", n, from, to);
		//b를 a을 임시저장소로 활용하여 c로 놓는다.
		HanioTab(n - 1, temp, from, to);
	}
}
// 1,처음 n-1개의 디스크를 A에서 C를 이용하여 B로 이동한다.
// 2,마지막 n번째 디스크를 A에서 C로 옮긴다.
// 3,n-1개의 디스크를 다시 B에서 A를 이용하여 C로 이동한다.
int g_iCounter = 0;
void HanioTower(int n, char from, char temp, char to)
{
	if (n == 1)
	{
		printf("[%d]번 %c -> %c\n", n, from, to);
		g_iCounter++;
	}
	else
	{
		//a를 c을 임시저장소로 활용하여 b로 놓는다.
		HanioTower(n - 1, from, to, temp);
		printf("[%d]번 %c -> %c\n", n, from, to);
		g_iCounter++;
		//b를 a을 임시저장소로 활용하여 c로 놓는다.
		HanioTower(n - 1, temp, from, to);
	}
}
//재귀호출
int factorial(int n) 
{
	if (n == 1)
	{
		return 1;
	}
	else
	{
		int iRet = factorial(n - 1);
		return n* iRet;
	}
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
void main()
{
	int sum = 1;
	// 3! = 3*2*1;
	// 5! = 5*4*3*2*1;
	for (int i = 1; i <= 3; i++)
	{
		sum = sum*i;
	}
	printf("\n%d", sum);
	printf("\n%d", factorial(3));

	int n;
	printf("\n원반갯수....?");
	scanf("%d", &n);
	//n = 20;
	//HanioTab(n,
	//	'A', // 시작 위치3

	//	'B', // 임시 위치
	//	'C');// 목표 위치
	HanioTower(n,
		'A', // 시작 위치
		'B', // 임시 위치
		'C');// 목표 위치
	printf("\n원반갯수(%d개)= %d번", n, g_iCounter);
}