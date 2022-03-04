#include "KQuadTree.h"


bool KQuadTree::Init(ID3D11DeviceContext* context, float width, float height)
{
	m_pContext = context;
	m_width = width;
	m_height = height;
	m_pRootNode = CreateNode(nullptr, 0, 0, m_width, m_height);
	Buildtree(m_pRootNode);
	KObject::CreateObject(L"../../data/shader/VSPS_Debug_0.txt",
		L"../../data/shader/VSPS_Debug_0.txt");
	return true;
}

void KQuadTree::Buildtree(KNode* pNode)
{
	if (pNode->m_rect.size.x >= 20.0f &&
		pNode->m_rect.size.y >= 20.0f)
	{
		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_rect.min.x,
			pNode->m_rect.min.y,
			pNode->m_rect.size.x / 2.0f,
			pNode->m_rect.size.y / 2.0f);
		Buildtree(pNode->m_pChild[0]);

		pNode->m_pChild[1] = CreateNode(pNode, pNode->m_pChild[0]->m_rect.max.x,
			pNode->m_pChild[0]->m_rect.min.y,
			pNode->m_pChild[0]->m_rect.size.x,
			pNode->m_pChild[0]->m_rect.size.y);
		Buildtree(pNode->m_pChild[1]);
		pNode->m_pChild[2] = CreateNode(pNode, pNode->m_pChild[0]->m_rect.max.x,
			pNode->m_pChild[0]->m_rect.max.y,
			pNode->m_pChild[0]->m_rect.size.x,
			pNode->m_pChild[0]->m_rect.size.y);
		Buildtree(pNode->m_pChild[2]);

		pNode->m_pChild[3] = CreateNode(pNode, pNode->m_pChild[0]->m_rect.min.x,
			pNode->m_pChild[0]->m_rect.max.y,
			pNode->m_pChild[0]->m_rect.size.x,
			pNode->m_pChild[0]->m_rect.size.y);
		Buildtree(pNode->m_pChild[3]);
	}
	else
	{
		m_pReafNode.push_back(pNode);
	}
}

bool KQuadTree::Release()
{
	if (m_pRootNode != nullptr)
	{
		delete m_pRootNode;
		m_pRootNode = nullptr;
	}
	return true;
}

KNode* KQuadTree::CreateNode(KNode* pParent, float x, float y, float w, float h)
{
	KNode* pNode = new KNode(x, y, w, h);

	//초기에 부모가 없으므로 예외처리
	if (pParent != nullptr)
	{
		pNode->m_depth = pParent->m_depth + 1;
		pNode->m_pParent = pParent;
	}

	return pNode;
}

KNode* KQuadTree::FindNode(KNode* pNode, KVector2 pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr &&
				pNode->m_pChild[iNode]->isRect(pos))
			{
				m_queue.push(pNode->m_pChild[iNode]);
				break;
			}
		}
		if (m_queue.empty())break;
		pNode = m_queue.front();
		m_queue.pop();
	} while (pNode);
	return pNode;
}

//Object CheckVertexData 함수 오버라이드해서
//버텍스 리스트 만든다.
bool KQuadTree::CheckVertexData()
{
	for (int i = 0; i < m_pReafNode.size(); i++)
	{
		PNCT_VERTEX vt;
		vt.pos = { m_pReafNode[i]->m_rect.min.x, m_pReafNode[i]->m_rect.min.y};
		vt.color = { 1,0,0,1 };
		m_VertexList.push_back(vt);

		vt.pos = { m_pReafNode[i]->m_rect.min.x, m_pReafNode[i]->m_rect.max.y };
		vt.color = { 1,0,0,1 };
		m_VertexList.push_back(vt);

		vt.pos = { m_pReafNode[i]->m_rect.max.x, m_pReafNode[i]->m_rect.max.y };
		vt.color = { 1,0,0,1 };
		m_VertexList.push_back(vt);

		vt.pos = { m_pReafNode[i]->m_rect.max.x, m_pReafNode[i]->m_rect.min.y };
		vt.color = { 1,0,0,1 };
		m_VertexList.push_back(vt);
	}
	return true;
}

bool KQuadTree::Render(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	KObject::Render(pContext);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

KQuadTree::KQuadTree()
{
	m_pRootNode = nullptr;
}

KQuadTree::~KQuadTree()
{
}
