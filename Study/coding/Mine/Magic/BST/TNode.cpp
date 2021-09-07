#include "TNode.h"
TNode::TNode() : m_iDepth(-1), m_iValue(-1)
{
	m_pChildNode[0] = nullptr;
	m_pChildNode[1] = nullptr;
}
TNode::TNode(int iValue) : m_iDepth(-1), m_iValue(iValue)
{
	m_pChildNode[0] = nullptr;
	m_pChildNode[1] = nullptr;
}
TNode::~TNode()
{
	int iDelData = this->m_iValue;
	if (m_pChildNode[0] != nullptr)
	{
		delete m_pChildNode[0];
		m_pChildNode[0] = nullptr;
	}
	if (m_pChildNode[1] != nullptr)
	{
		delete m_pChildNode[1];
		m_pChildNode[1] = nullptr;
	}
	std::cout << "Current :" << iDelData << "Delete!\n";
}

void TNode::AddBinaryNode(TNode* pNode, int iValue)
{
	if (pNode->m_iValue > iValue)
	{
		if (pNode->m_pChildNode[0] == nullptr)
		{
			pNode->m_pChildNode[0] = new TNode(iValue);
			//pNode->m_pChildNode[0]->m_iValue = iValue;
			pNode->m_pChildNode[0]->m_iDepth = pNode->m_iDepth + 1;
		}
		else
		{
			AddBinaryNode(pNode->m_pChildNode[0], iValue);
		}
	}
	else
	{
		if (pNode->m_pChildNode[1] == nullptr)
		{
			pNode->m_pChildNode[1] = new TNode(iValue);
			//pNode->m_pChildNode[1]->m_iValue = iValue;
			pNode->m_pChildNode[1]->m_iDepth = pNode->m_iDepth + 1;
		}
		else
		{
			AddBinaryNode(pNode->m_pChildNode[1], iValue);
		}
	}
}
void TNode::DeleteAll(TNode* pNode)
{
	TNode* pLeft = nullptr;
	TNode* pRight = nullptr;
	if (pNode != nullptr)
	{
		pLeft = pNode->m_pChildNode[0];
		pRight = pNode->m_pChildNode[1];
		delete pNode;
	}
	if (pLeft != nullptr)
	{
		DeleteAll(pLeft);
	}
	if (pRight != nullptr)
	{
		DeleteAll(pRight);
	}
}
TNode* TNode::Find(TNode* pNode, int iValue)
{
	if (pNode->m_iValue == iValue)
	{
		return pNode;
	}
	if (pNode->m_iValue > iValue)
	{
		if (pNode->m_pChildNode[0] != nullptr)
		{
			return Find(pNode->m_pChildNode[0], iValue);
		}
	}
	else
	{
		if (pNode->m_pChildNode[1] != nullptr)
		{
			return Find(pNode->m_pChildNode[1], iValue);
		}
	}
	return nullptr;
}