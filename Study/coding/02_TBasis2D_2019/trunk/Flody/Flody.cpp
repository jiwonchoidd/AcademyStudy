// Flody.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "limits.h"
#include <windows.h>

#define VERTEX 4
#define INF 999

int weight[VERTEX][VERTEX] = {
	0,3,INF, INF,
	INF, 0, 12, 6,
	INF, 5, 0,7,
	2,9,4,0,
};
int dist[VERTEX][VERTEX];

void print(int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (dist[i][j] == INF)
				printf("INF ");
			else
				printf("%3d", dist[i][j]);

		}
		printf("\n");
	}

}

void floyd(int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			dist[i][j] = weight[i][j];
		}
	}
	for (int k = 0; k < n; k++)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				printf("[%d][%d]%d + [%d][%d]%d < [%d][%d]%d\n", 
					i,k,dist[i][k], 
					k, j, dist[k][j],
					i,j,
					  dist[i][j]);
				if (dist[i][k] + dist[k][j] < dist[i][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
		printf("���� %d�� ��ġ�� ���\n", k);		
		print(n);
		printf("�ƹ�Ű�� �Է��Ͻÿ�!");
		getchar();
		system("cls");		
	}
}
int main()
{
	floyd(VERTEX);
	print(VERTEX);
    return 0;
}

