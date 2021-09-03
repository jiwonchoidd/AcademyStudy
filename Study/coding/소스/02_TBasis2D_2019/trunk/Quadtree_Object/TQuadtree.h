#pragma once
#include "TMap.h"
#include "TObjectBox.h"

class TNode
{
public:
	DWORD    m_dwDepth;
	T_Box    m_box;
	T_Sphere m_Sphere;
	bool    m_bLeafNode;
	DWORD    m_dwConer[4];
	TNode*   m_pChild[4];
	TNode*   m_pParent;
	vector<TMapObj*> m_ObjList;
public:
	TNode();
	virtual ~TNode();
};
class TQuadtree : public TMap
{
public:
	TNode*		m_pRootNode;	
	vector<TNode*>  m_RenderList;
public:
	bool		BuildTree(TNode* pNode);
	bool		SubDivide(TNode* pNode);
	TNode*      CreateNode(TNode* pParent, DWORD a,
		DWORD b, DWORD c,   DWORD d);
	bool		Frame();
	void		DrawFindNode(TNode* pNode);
	int			UpdateIndexList(DWORD dwCurrentIndex,
		DWORD u_left, DWORD u_right,
		DWORD b_right, DWORD b_left);
	void        AddObject(TMapObj*  obj);
	void		AddObject(TNode* pNode, TMapObj* pObj);
	TNode*		FindNode(TNode* pNode, 
					TMapObj* pObj);
	T_POSITION	CheckRect(TNode* pNode, TMapObj* pObj);
	int			DrawChild(TNode* pNode);
public:
	TQuadtree();
	virtual ~TQuadtree();
};

