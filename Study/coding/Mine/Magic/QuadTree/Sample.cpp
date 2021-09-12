#include "TQuadtree.h"
int main()
{
	//°´Ã¼ »ý¼º
	TQuadtree g_TQT;
	g_TQT.Init(100, 100);
	for (int iObj = 0; iObj < 3; iObj++)
	{
		TVector pos;
		pos.x = (float)(rand() % 100);
		pos.x = (float)(rand() % 100);
		g_TQT.AddObject(pos);
	}

	//g_TQT.Release();
	cout << "hello world\n";
}