#include "KOctree.h"
#include <windows.h>
/*Octree�� ����� �������� ���� �� ���� ��ü��
���� �� �̵� ó���� �����Ѵ�.*/
int main()
{
	KOctree g_Octree;
	g_Octree.Init(100, 100);
	//������Ʈ �������� ��ġ �Ҵ�
	for (int iObj = 0; iObj < 3; iObj++)
	{
		KVector3 pos;
		pos.x = (float)(rand() % 100);
		pos.y= (float)(rand() % 100);
		pos.z = (float)(rand() % 100);
		if (g_Octree.AddObject(pos))
		{
			std::cout << pos.x << ", " << pos.y << ", " << pos.z << " �� ������Ʈ�� �߰��Ǿ���\n";
		}
	}
	float fSec = 0.0f;

	//������Ʈ ���� ������
	while (fSec < 30)
	{
		Sleep(10);
		float time = 10.0f / 1000.0f;
		g_Octree.Frame(time);
		fSec += 10.0f / 1000.0f;
	}

	std::cout << "\nEnd"<<std::endl;
	KNode::g_iNewCounter--;
	g_Octree.Release();
	//
}

