#include "OctTree.h"
#include <windows.h>
/*Octree가 적용된 지형에서 정적 및 동적 개체의
제외 및 이동 처리를 구현한다.*/
int main()
{
	OctTree g_Octree;
	g_Octree.Init(100, 100);
	//오브젝트 랜덤으로 위치 할당
	for (int iObj = 0; iObj < 3; iObj++)
	{
		TVector pos;
		pos.x = (float)(rand() % 100);
		pos.y= (float)(rand() % 100);
		pos.z = (float)(rand() % 100);
		std::cout << pos.x<<", "<< pos.y << ", " << pos.z << " 에 오브젝트가 추가되었음\n";
		//
		g_Octree.AddObject(pos);
	}
	float fSec = 0.0f;
	//오브젝트 동적 움직임
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

