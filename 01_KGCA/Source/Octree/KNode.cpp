#include "KNode.h"
int   KNode::g_iNewCounter = 0;
bool KNode::AddObject(KVector pos)
{
	m_ObjectList.push_back(pos);
	return true;
}
bool KNode::AddObject(float fX, float fY, float fZ)
{
	m_ObjectList.push_back(KVector(fX, fY, fZ));
	return true;
}
bool KNode::isRect(KVector pos)
{
	if (this->m_KRect.p0.x <= pos.x &&
		this->m_KRect.p0.y <= pos.y &&
		this->m_KRect.p0.z <= pos.z &&
		this->m_KRect.p1.x >= pos.x &&
		this->m_KRect.p1.y >= pos.y &&
		this->m_KRect.p1.z >= pos.z)
	{
		return true;
	}
	else
		return false;
}

KNode::KNode(float x, float y, float z, float w, float h)
{
	m_pParent = nullptr;
	m_iDepth = 0;
	this->m_KRect.wh = KVector(w, h, w);
	this->m_KRect.p0 = KVector(x, y, z);
	//박스를 만들기 위해서 피좌표는 z 축은 정육면체니까 w와 동일
	this->m_KRect.p1 = this->m_KRect.p0 + KVector(w, h, w);
}

KNode::~KNode()
{
	for (int iChild = 0; iChild < 8; iChild++)
	{
		if (m_pChild[iChild] != nullptr)
		{
			delete m_pChild[iChild];
			m_pChild[iChild] = nullptr;
			KNode::g_iNewCounter--;
		}
	}
}