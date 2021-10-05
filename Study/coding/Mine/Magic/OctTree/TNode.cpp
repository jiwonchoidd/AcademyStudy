#include "TNode.h"
int   TNode::g_iNewCounter = 0;
bool TNode::AddObject(TVector pos)
{
	m_ObjectList.push_back(pos);
	return true;
}
bool TNode::AddObject(float fX, float fY, float fZ)
{
	m_ObjectList.push_back(TVector(fX, fY, fZ));
	return true;
}
bool TNode::isRect(TVector pos)
{
	if (this->m_TRect.p0.x <= pos.x &&
		this->m_TRect.p0.y <= pos.y &&
		this->m_TRect.p0.z <= pos.z &&
		this->m_TRect.p1.x >= pos.x &&
		this->m_TRect.p1.y >= pos.y &&
		this->m_TRect.p1.z >= pos.z)
	{
		return true;
	}
	else
		return false;
}

TNode::TNode(float x, float y, float z, float w, float h)
{
	m_pParent = nullptr;
	m_iDepth = 0;
	this->m_TRect.wh = TVector(w, h, w);
	this->m_TRect.p0 = TVector(x, y, z);
	//박스를 만들기 위해서 피좌표는 z 축은 정육면체니까 w와 동일
	this->m_TRect.p1 = this->m_TRect.p0 + TVector(w, h, w);
}

TNode::~TNode()
{
	for (int iChild = 0; iChild < 8; iChild++)
	{
		if (m_pChild[iChild] != nullptr)
		{
			delete m_pChild[iChild];
			m_pChild[iChild] = nullptr;
			TNode::g_iNewCounter--;
		}
	}
}