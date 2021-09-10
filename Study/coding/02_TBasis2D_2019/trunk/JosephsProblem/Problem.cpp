#include "Problem.h"

///////////////////////////////////////////////////////////////////
// 원형링크드리스트를 작성한다.
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
// 노드를 삭제한다.
// pNode는 삭제 하려는 노드의 이전 노드이어야 한다.
// 이유는 단일 연결리스트에서는 이전 노드를 알 수 없기 때문이다.
///////////////////////////////////////////////////////////////////
_NODE * NextNodeDelete(_NODE * pNode )
{
	_NODE *pDelNode	= pNode->pNext;
	pNode->pNext	= pDelNode->pNext;
	free(pDelNode);
	return pNode->pNext;
}
///////////////////////////////////////////////////////////////////
// 요셉의 문제를 계산한다.
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
		printf("\n[%c]열외", pNode->pNext->cName);
		pNode = NextNodeDelete(pNode);
	}
	printf( "\n탈출할 장병 : %c\n", pNode->cName );
	free(pNode);
}
///////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////
void main()
{
	char	strList[100];
	int		iCount;
	printf("장병 문자열...?");
	scanf("%s", strList);
	//fflush(stdin);
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	printf("몇명 간격  ...?");
	scanf("%d", &iCount);

	_NODE* pLeafNode = CircularLinkedList(strList);

	JosephProblem(pLeafNode, iCount);

	_getch();
}