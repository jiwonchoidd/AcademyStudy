#include "KOctree.h"
void	KOctree::Frame(float time)
{
	Tobj.Velocity.x = 10.0f;
	Tobj.Velocity.y = 10.0f;
	Tobj.Velocity.z = 10.0f;
	Tobj.pos += Tobj.Velocity * time;

	KNode* pFind = FindPlayerNode(Tobj.pos);
	if (pFind != nullptr)
	{
		std::cout << " [ index- " << pFind->m_iIndex<<" x : "<< pFind->m_KRect.p0.x << " y : "<<
			pFind->m_KRect.p0.y<<" z : " << pFind->m_KRect.p0.z << " ] \n";
	}
}
KNode* KOctree::FindPlayerNode(KVector pos)
{
	KNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		return pFindNode;
	}
	return nullptr;
}
KNode* KOctree::FindNode(KNode* pNode, KVector pos)
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
bool KOctree::Init(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pRootNode = CreateNode(nullptr, 0, 0, 0, iWidth, iHeight);
	Buildtree(m_pRootNode);
	return true;
}

void KOctree::Buildtree(KNode* pNode)
{
	if (pNode->m_KRect.wh.x >= 30.0f &&
		pNode->m_KRect.wh.y >= 30.0f &&
		pNode->m_KRect.wh.z >= 30.0f)
	{
		for (int ZAxis = 0; ZAxis < 2; ZAxis++)
		{
			if (ZAxis == 1)
			{
				pNode->m_pChild[4] = CreateNode(pNode, pNode->m_KRect.p0.x,
					pNode->m_KRect.p0.y, pNode->m_KRect.p1.z/2,
					pNode->m_KRect.wh.x / 2.0f,
					pNode->m_KRect.wh.y / 2.0f);
				Buildtree(pNode->m_pChild[4]);
				//
				pNode->m_pChild[5] = CreateNode(pNode, pNode->m_pChild[0]->m_KRect.p1.x,
					pNode->m_pChild[0]->m_KRect.p0.y, pNode->m_KRect.p1.z/2,
					pNode->m_pChild[0]->m_KRect.wh.x,
					pNode->m_pChild[0]->m_KRect.wh.y);
				Buildtree(pNode->m_pChild[5]);

				pNode->m_pChild[6] = CreateNode(pNode, pNode->m_pChild[0]->m_KRect.p1.x,
					pNode->m_pChild[0]->m_KRect.p1.y, pNode->m_KRect.p1.z/2,
					pNode->m_pChild[0]->m_KRect.wh.x,
					pNode->m_pChild[0]->m_KRect.wh.y);
				Buildtree(pNode->m_pChild[6]);

				pNode->m_pChild[7] = CreateNode(pNode, pNode->m_pChild[0]->m_KRect.p0.x,
					pNode->m_pChild[0]->m_KRect.p1.y, pNode->m_KRect.p1.z/2,
					pNode->m_pChild[0]->m_KRect.wh.x,
					pNode->m_pChild[0]->m_KRect.wh.y);
				Buildtree(pNode->m_pChild[7]);
			}
			else 
			{
				pNode->m_pChild[0] = CreateNode(pNode, pNode->m_KRect.p0.x,
					pNode->m_KRect.p0.y, pNode->m_KRect.p0.z,
					pNode->m_KRect.wh.x / 2.0f,
					pNode->m_KRect.wh.y / 2.0f);
				Buildtree(pNode->m_pChild[0]);
				//
				pNode->m_pChild[1] = CreateNode(pNode, pNode->m_pChild[0]->m_KRect.p1.x,
					pNode->m_pChild[0]->m_KRect.p0.y, pNode->m_KRect.p0.z,
					pNode->m_pChild[0]->m_KRect.wh.x,
					pNode->m_pChild[0]->m_KRect.wh.y);
				Buildtree(pNode->m_pChild[1]);

				pNode->m_pChild[2] = CreateNode(pNode, pNode->m_pChild[0]->m_KRect.p1.x,
					pNode->m_pChild[0]->m_KRect.p1.y, pNode->m_KRect.p0.z,
					pNode->m_pChild[0]->m_KRect.wh.x,
					pNode->m_pChild[0]->m_KRect.wh.y);
				Buildtree(pNode->m_pChild[2]);

				pNode->m_pChild[3] = CreateNode(pNode, pNode->m_pChild[0]->m_KRect.p0.x,
					pNode->m_pChild[0]->m_KRect.p1.y, pNode->m_KRect.p0.z,
					pNode->m_pChild[0]->m_KRect.wh.x,
					pNode->m_pChild[0]->m_KRect.wh.y);
				Buildtree(pNode->m_pChild[3]);
			}
		}
	}
}

KNode* KOctree::CreateNode(KNode* pParent, float x, float y, float z, float w, float h)
{
	KNode* pNode = new KNode(x, y, z, w, h);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}
	pNode->m_iIndex = KNode::g_iNewCounter;
	KNode::g_iNewCounter++;
	return pNode;
}

bool KOctree::AddObject(KVector pos)
{
	KNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}


void KOctree::Release()
{
	delete m_pRootNode;
	m_pRootNode = nullptr;
}

KOctree::KOctree()
{
	m_pRootNode = nullptr;
}

KOctree::~KOctree()
{
}
