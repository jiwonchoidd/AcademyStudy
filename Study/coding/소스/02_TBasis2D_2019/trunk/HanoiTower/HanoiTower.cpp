//////////////////////  ���ȣ��/////////////////////////
// ����� ���� �Լ����
// if()
// ���ȣ��
//////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
int iCount = 0;
///////////////////////////////////////////////////////////////////
// 1,��ũ�� �ѹ��� �ϳ��� �Űܾ� �Ѵ�.
// 2,���� ���� ��ũ�� �Ʒ����� ��ũ���� Ŭ �� ����.
//  n
// 2  - 1  �� �̵��Ѵ�.
///////////////////////////////////////////////////////////////////
void HanioTab(int n, char from, char temp, char to)
{
	if (n > 0)
	{
		//a�� c�� �ӽ�����ҷ� Ȱ���Ͽ� b�� ���´�.
		HanioTab(n - 1, from, to, temp);
		printf("[%d]�� %c -> %c\n", n, from, to);
		//b�� a�� �ӽ�����ҷ� Ȱ���Ͽ� c�� ���´�.
		HanioTab(n - 1, temp, from, to);
	}
}
// 1,ó�� n-1���� ��ũ�� A���� C�� �̿��Ͽ� B�� �̵��Ѵ�.
// 2,������ n��° ��ũ�� A���� C�� �ű��.
// 3,n-1���� ��ũ�� �ٽ� B���� A�� �̿��Ͽ� C�� �̵��Ѵ�.
int g_iCounter = 0;
void HanioTower(int n, char from, char temp, char to)
{
	if (n == 1)
	{
		printf("[%d]�� %c -> %c\n", n, from, to);
		g_iCounter++;
	}
	else
	{
		//a�� c�� �ӽ�����ҷ� Ȱ���Ͽ� b�� ���´�.
		HanioTower(n - 1, from, to, temp);
		printf("[%d]�� %c -> %c\n", n, from, to);
		g_iCounter++;
		//b�� a�� �ӽ�����ҷ� Ȱ���Ͽ� c�� ���´�.
		HanioTower(n - 1, temp, from, to);
	}
}
//���ȣ��
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
	printf("\n���ݰ���....?");
	scanf("%d", &n);
	//n = 20;
	//HanioTab(n,
	//	'A', // ���� ��ġ3

	//	'B', // �ӽ� ��ġ
	//	'C');// ��ǥ ��ġ
	HanioTower(n,
		'A', // ���� ��ġ
		'B', // �ӽ� ��ġ
		'C');// ��ǥ ��ġ
	printf("\n���ݰ���(%d��)= %d��", n, g_iCounter);
}