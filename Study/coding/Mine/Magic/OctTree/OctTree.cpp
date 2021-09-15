#include "OctTree.h"
void	OctTree::Frame(float time)
{
	Tobj.Velocity.x = 10.0f;
	Tobj.Velocity.y = 0.0f;
	Tobj.Velocity.z = 0.0f;
	Tobj.pos += Tobj.Velocity * time;
	TNode* pFind = FindPlayerNode(Tobj.pos);
	if (pFind != nullptr)
	{
		std::cout << pFind->m_iIndex << " ";
	}
}
TNode* OctTree::FindPlayerNode(TVector pos)
{
	TNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		return pFindNode;
	}
	return nullptr;
}
bool OctTree::Init(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pRootNode = CreateNode(nullptr, 0, 0, 0, iWidth, iHeight);
	Buildtree(m_pRootNode);
	return true;
}

void OctTree::Buildtree(TNode* pNode)
{
	if (pNode->m_TRect.wh.x >= 30.0f &&
		pNode->m_TRect.wh.y >= 30.0f&&
		pNode->m_TRect.wh.z >= 30.0f)
	{
		for (int ZAxis = 0; ZAxis < 2; ZAxis++)
		{
			if (ZAxis == 1)
			{
				pNode->m_pChild[4] = CreateNode(pNode, pNode->m_TRect.p0.x,
					pNode->m_TRect.p0.y, pNode->m_TRect.p1.z/2,
					pNode->m_TRect.wh.x / 2.0f,
					pNode->m_TRect.wh.y / 2.0f);
				Buildtree(pNode->m_pChild[4]);
				//
				pNode->m_pChild[5] = CreateNode(pNode, pNode->m_pChild[0]->m_TRect.p1.x,
					pNode->m_pChild[0]->m_TRect.p0.y, pNode->m_TRect.p1.z/2,
					pNode->m_pChild[0]->m_TRect.wh.x,
					pNode->m_pChild[0]->m_TRect.wh.y);
				Buildtree(pNode->m_pChild[5]);

				pNode->m_pChild[6] = CreateNode(pNode, pNode->m_pChild[0]->m_TRect.p1.x,
					pNode->m_pChild[0]->m_TRect.p1.y, pNode->m_TRect.p1.z/2,
					pNode->m_pChild[0]->m_TRect.wh.x,
					pNode->m_pChild[0]->m_TRect.wh.y);
				Buildtree(pNode->m_pChild[6]);

				pNode->m_pChild[7] = CreateNode(pNode, pNode->m_pChild[0]->m_TRect.p0.x,
					pNode->m_pChild[0]->m_TRect.p1.y, pNode->m_TRect.p1.z/2,
					pNode->m_pChild[0]->m_TRect.wh.x,
					pNode->m_pChild[0]->m_TRect.wh.y);
				Buildtree(pNode->m_pChild[7]);
			}
			else 
			{
				pNode->m_pChild[0] = CreateNode(pNode, pNode->m_TRect.p0.x,
					pNode->m_TRect.p0.y, pNode->m_TRect.p0.z,
					pNode->m_TRect.wh.x / 2.0f,
					pNode->m_TRect.wh.y / 2.0f);
				Buildtree(pNode->m_pChild[0]);
				//
				pNode->m_pChild[1] = CreateNode(pNode, pNode->m_pChild[0]->m_TRect.p1.x,
					pNode->m_pChild[0]->m_TRect.p0.y, pNode->m_TRect.p0.z,
					pNode->m_pChild[0]->m_TRect.wh.x,
					pNode->m_pChild[0]->m_TRect.wh.y);
				Buildtree(pNode->m_pChild[1]);

				pNode->m_pChild[2] = CreateNode(pNode, pNode->m_pChild[0]->m_TRect.p1.x,
					pNode->m_pChild[0]->m_TRect.p1.y, pNode->m_TRect.p0.z,
					pNode->m_pChild[0]->m_TRect.wh.x,
					pNode->m_pChild[0]->m_TRect.wh.y);
				Buildtree(pNode->m_pChild[2]);

				pNode->m_pChild[3] = CreateNode(pNode, pNode->m_pChild[0]->m_TRect.p0.x,
					pNode->m_pChild[0]->m_TRect.p1.y, pNode->m_TRect.p0.z,
					pNode->m_pChild[0]->m_TRect.wh.x,
					pNode->m_pChild[0]->m_TRect.wh.y);
				Buildtree(pNode->m_pChild[3]);
			}
		}
	}
}

TNode* OctTree::CreateNode(TNode* pParent, float x, float y, float z, float w, float h)
{
	TNode* pNode = new TNode(x, y, z, w, h);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}
	return pNode;
}

bool OctTree::AddObject(TVector pos)
{
	TNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}

TNode* OctTree::FindNode(TNode* pNode, TVector pos)
{
	do {
		for (int iNode = 0; iNode < 8; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr &&
				pNode->m_pChild[iNode]->isRect(pos))
			{
				m_Queue.push(pNode->m_pChild[iNode]);
				break;
			}
		}
		if (m_Queue.empty())break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);
	return pNode;
}

void OctTree::Release()
{
	delete m_pRootNode;
	m_pRootNode = nullptr;
}

OctTree::OctTree()
{
	m_pRootNode = nullptr;
}

OctTree::~OctTree()
{
}
