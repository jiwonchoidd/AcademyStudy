#pragma once
#include "TMap.h"
#include "TObjectBox.h"

class TNode
{
public:
	DWORD		m_dwDepth;
	DWORD		m_dwFaces;
	T_Box		m_box;
	T_Sphere	m_Sphere;
	bool		m_bLeafNode;
	DWORD		m_dwConer[4];
	TNode*		m_pChild[4];
	TNode*		m_pParent;
	TNode*		m_pNeighborList[4];
	DWORD		m_dwPositionIndex[2];
	// 인덱스 버퍼
	vector<DWORD> m_IndexList;
	vector<PNCT_VERTEX> m_VertexList;
	TDxBuffer	m_IB;
	TDxBuffer	m_VB;
	vector<TMapObj*> m_ObjList;
	DWORD		m_dwLodLevel;
	DWORD		m_dwLodType;
public:
	TNode();
	virtual ~TNode();
};


class TQuadtree : public TMap
{
public:
	TNode*					m_pRootNode;	
	vector<TNode*>			m_RenderList;
	typedef vector<TNode*>	TNODE_VECTOR;
	vector< TNODE_VECTOR >  m_LevelList;
	DWORD					m_dwNumDepth;
public:
	bool		BuildTree(TNode* pNode);
	bool		SubDivide(TNode* pNode);
	TNode*      CreateNode(TNode* pParent, DWORD a,	DWORD b, DWORD c,   DWORD d);
	bool		Frame();
	void		DrawFindNode(TNode* pNode);
	int			UpdateIndexList(DWORD dwCurrentIndex,DWORD u_left, DWORD u_right,	DWORD b_right, DWORD b_left);
	void        AddObject(TMapObj*  obj);
	void		AddObject(TNode* pNode, TMapObj* pObj);
	TNode*		FindNode(TNode* pNode, 	TMapObj* pObj);
	T_POSITION	CheckRect(TNode* pNode, TMapObj* pObj);
	int			DrawChild(TNode* pNode);
	// 가상함수
	virtual bool	CreateBuffer(TNode* pNode) { return true; };
public:
	TQuadtree();
	virtual ~TQuadtree();
};

