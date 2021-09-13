#include "TNode.h"

bool TNode::AddObject(TVector pos)
{
	m_ObjectList.push_back(pos);
	return true;
}

bool TNode::isRect(TVector pos)
{
	if (this->m_TRect.p0.x <= pos.x &&
		this->m_TRect.p0.y <= pos.y &&
		this->m_TRect.p1.x >= pos.x &&
		this->m_TRect.p1.y >= pos.y)
	{
		return true;
	}
	else
		return false;
}

TNode::TNode(float x, float y, float w, float h)
{
	m_pParent = nullptr;
	m_iDepth = 0;
	this->m_TRect.wh = TVector(w, h);
	this->m_TRect.p0 = TVector(x, y);
	this->m_TRect.p1 =
		this->m_TRect.p0 + TVector(w, h);
}

TNode::~TNode()
{
	for (int iChild = 0; iChild < 8; iChild++)
	{
		if (m_pChild[iChild] != nullptr)
		{
			delete m_pChild[iChild];
			m_pChild[iChild] = nullptr;
		}
	}
}