#pragma once
#include "TShape.h"
class TObjectBox : public TBoxShape
{
public:
	TObjectBox();
	virtual ~TObjectBox();
};


struct TMapObj
{
	D3DXMATRIX  m_matWorld;
	T_Box		m_tBox;
	bool		m_bDraw;
	bool		m_bCollision;
	TObjectBox*     m_pBaseShape;
	void Set(TObjectBox* pBaseShape, DWORD dwWidth, DWORD dwHeight);
	void Draw(D3DXMATRIX& matView, D3DXMATRIX& matProj) 
	{ 
		m_pBaseShape->SetMatrix(m_matWorld, matView, matProj);
		m_pBaseShape->Render(); 
		m_bDraw = true;
	}
};


