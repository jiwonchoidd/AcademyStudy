#include "OctTree.h"
#include <windows.h>
/*Octree�� ����� �������� ���� �� ���� ��ü��
���� �� �̵� ó���� �����Ѵ�.*/
int main()
{
	OctTree g_Octree;
	g_Octree.Init(100, 100);
	//������Ʈ �������� ��ġ �Ҵ�
	for (int iObj = 0; iObj < 3; iObj++)
	{
		TVector pos;
		pos.x = (float)(rand() % 100);
		pos.y= (float)(rand() % 100);
		pos.z = (float)(rand() % 100);
		std::cout << pos.x<<", "<< pos.y << ", " << pos.z << " �� ������Ʈ�� �߰��Ǿ���\n";
		//
		g_Octree.AddObject(pos);
	}
	float fSec = 0.0f;
	//������Ʈ ���� ������
	while (fSec < 10)
	{
		Sleep(10);
		g_Octree.Frame(10.0f / 1000.0f);
		fSec += 10.0f / 1000.0f;
	}

	std::cout << "end"<<std::endl;
	TNode::g_iNewCounter--;
	g_Octree.Release();
	//
}

