#include "Problem.h"

///////////////////////////////////////////////////////////////////
// ������ũ�帮��Ʈ�� �ۼ��Ѵ�.
///////////////////////////////////////////////////////////////////
_NODE* CircularLinkedList( char *strList )
{
	_NODE *pNode=0;	

	pNode			=	(_NODE*)malloc(sizeof(_NODE));
	pNode->cName	=	strList[0];
	g_pHead			=	pNode;

	int iMaxCount = strlen(strList);
	for( int iCount = 1; iCount < iMaxCount; iCount++)
	{
		pNode->pNext	=	(_NODE*)malloc(sizeof(_NODE));
		pNode			=	pNode->pNext;
		pNode->cName	=	strList[iCount];
	}
	pNode->pNext = g_pHead;
	return g_pHead;
}
///////////////////////////////////////////////////////////////////
// ��带 �����Ѵ�.
// pNode�� ���� �Ϸ��� ����� ���� ����̾�� �Ѵ�.
// ������ ���� ���Ḯ��Ʈ������ ���� ��带 �� �� ���� �����̴�.
///////////////////////////////////////////////////////////////////
_NODE * NextNodeDelete(_NODE * pNode )
{
	_NODE *pDelNode	= pNode->pNext;
	pNode->pNext	= pDelNode->pNext;
	free(pDelNode);
	return pNode->pNext;
}
///////////////////////////////////////////////////////////////////
// ����� ������ ����Ѵ�.
///////////////////////////////////////////////////////////////////
void JosephProblem( _NODE *pNode, int n )
{
	_NODE *pPreNode = 0;
	while( pNode != pNode->pNext )
	{
		for( int i = 1; i < n-1; i++ )
		{
			pNode = pNode->pNext;
		}
		printf("\n[%c]����", pNode->pNext->cName);
		pNode = NextNodeDelete(pNode);
	}
	printf( "\nŻ���� �庴 : %c\n", pNode->cName );
	free(pNode);
}
///////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////
void main()
{
	char	strList[100];
	int		iCount;
	printf("�庴 ���ڿ�...?");
	scanf("%s", strList);
	//fflush(stdin);
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	printf("��� ����  ...?");
	scanf("%d", &iCount);

	_NODE* pLeafNode = CircularLinkedList(strList);

	JosephProblem(pLeafNode, iCount);

	_getch();
}