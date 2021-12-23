#include "KOctree.h"

bool KOctree::Init(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pRootNode = CreateNode(nullptr, 0, 0, 0, iWidth, iHeight);
	Buildtree(m_pRootNode);
	return true;
}
void	KOctree::Frame(float time)
{
	m_Movableobj.Velocity.x = 10.0f;
	m_Movableobj.Velocity.y = 10.0f;
	m_Movableobj.Velocity.z = 10.0f;

	//옥트리 영역 나갔을때 방향 재설정하기
	if (m_Movableobj.pos.x >= 100 || m_Movableobj.pos.y >= 100 || m_Movableobj.pos.z >= 100)
	{
		m_Movableobj.dir = -1.0f;
		std::cout << "\n<-방향 재설정(↙)\n";
	}
	else if(m_Movableobj.pos.x <= 0 || m_Movableobj.pos.y <= 0 || m_Movableobj.pos.z <= 0)
	{
		m_Movableobj.dir = +1.0f;
		std::cout << "\n->방향 재설정(↗)\n";
	}
	//움직임 연산
	m_Movableobj.pos += m_Movableobj.Velocity* m_Movableobj.dir * time;
	
	//해당 위치 노드를 찾아준다.
	KNode* pFind = FindPlayerNode(m_Movableobj.pos);

	if (pFind != nullptr && m_Movableobj.pos.x<100)
	{
		KVector3 k;
		//p0, p1의 중앙 지점을 찾아준다.(x1+x2)/2, (y1+y2)/2
		k= k.MiddleVector(pFind->m_KRect.p0, pFind->m_KRect.p1);
		
		//이전과 같은 노드 안에 있을때 출력 안함
		if (m_Mp.x != k.x)
		{
		std::cout << "\n [ Current Node - " << pFind->m_iIndex<<", X : "<< k.x << ", Y : "<<
			k.y<<", Z : " << k.y<< " ]\n" <<std::endl;
		}
		else
		{
			std::string sdir = (m_Movableobj.dir > 0) ? "↗" : "↙";
			std::cout <<sdir;
		}
		m_Mp = k;
	}
}
void KOctree::Move(KNode* pNode, KVector3 pos)
{
}
KNode* KOctree::FindPlayerNode(KVector3 pos)
{
	KNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		return pFindNode;
	}
	return nullptr;
}
KNode* KOctree::FindNode(KNode* pNode, KVector3 pos)
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

bool KOctree::AddObject(KVector3 pos)
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
