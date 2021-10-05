#include "TNode.h"
#include <iostream>
int   g_iData[10] = { 5,0,4,7,8,3,9,2,6,1 };
TNode* g_pRoot = nullptr;
int main()
{
	TNode myNode;
	g_pRoot = new TNode(g_iData[0]);
	//g_pRoot->m_iValue = g_iData[0];
	g_pRoot->m_iDepth = 0;
	for (int i = 1; i < 10; i++)
	{
		myNode.AddBinaryNode(g_pRoot, g_iData[i]);
	}

	TNode* pFindNode = myNode.Find(g_pRoot, 8);
	if (pFindNode != nullptr)
	{
		std::cout << pFindNode->m_iValue << "Hello World!\n";
	}
	else
	{
		std::cout << "Not! Finder!\n";
	}
	delete g_pRoot;
	g_pRoot = nullptr;
	myNode.DeleteAll(g_pRoot);
}