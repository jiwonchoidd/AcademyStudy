#include <iostream>
class TNode
{
public :
	int	   m_iDepth;
	int    m_iValue;
	TNode* m_pChildNode[2];
	TNode();
	TNode(int iValue);
	~TNode();
	void AddBinaryNode(TNode* pNode, int iValue);
	TNode* Find(TNode* pNode, int iFind);
	void DeleteAll(TNode* pNode);
};

