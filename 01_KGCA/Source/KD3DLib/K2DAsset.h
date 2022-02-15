#pragma once
#include "KObject.h"
#include "KCollider.h"

class K2DAsset : public KObject
{
public:
	KCollider*			m_coll;
	float				m_speed;
public:
	RECT				m_rtSource;
	RECT				m_rtDraw;
public:
	virtual void		SetRectSource(RECT rt);
	virtual void		SetRectDraw(RECT rt);
	virtual void		AddPosition(KVector2 vPos, ID3D11DeviceContext* pContext);
	virtual void		SetPosition(KVector2 vPos);
	virtual void		UpdateRectDraw(RECT rt);
	virtual bool		CreateObject_Mask(std::wstring vsFile,
		std::wstring psFile, std::wstring tex, std::wstring mask);
	// 화면좌표 위치를 중점으로 NDC 변환
	virtual void	Convert(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	Convert(std::vector<PNCT_VERTEX>& list,
		std::vector<PNCT_VERTEX>& retList);
public:
	virtual bool		SetVertexData();
	virtual bool		CheckVertexData() override;
public:
	K2DAsset() 
	{
		m_speed=20.0f;
		m_rtSource = {0,0};
		m_rtDraw={ 0,0 };
		
	};
	~K2DAsset()
	{

	};
};

