#include "KOctree.h"
#include <windows.h>
/*Octree가 적용된 지형에서 정적 및 동적 개체의
제외 및 이동 처리를 구현한다.*/
int main()
{
	KOctree g_Octree;
	g_Octree.Init(100, 100);
	//오브젝트 랜덤으로 위치 할당
	for (int iObj = 0; iObj < 3; iObj++)
	{
		KVector3 pos;
		pos.x = (float)(rand() % 100);
		pos.y= (float)(rand() % 100);
		pos.z = (float)(rand() % 100);
		if (g_Octree.AddObject(pos))
		{
			std::cout << pos.x << ", " << pos.y << ", " << pos.z << " 에 오브젝트가 추가되었음\n";
		}
	}
	float fSec = 0.0f;

	//오브젝트 동적 움직임
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

