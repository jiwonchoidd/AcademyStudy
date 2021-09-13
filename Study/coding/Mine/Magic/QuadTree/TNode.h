#include "TStd.h"
class TNode
{
public:
	TRect	m_tRect;
	vector<TVector> m_ObjectList;
	TNode* m_pChild[4];
	TNode* m_pParent;
	int m_iDepth;
public: 
	bool AddObject(float fX, float fY);
	bool AddObject(TVector pos);
	bool isRect(TVector pos);
public:
	TNode() {};
	TNode(float x, float y, float w, float h);
	~TNode();
};