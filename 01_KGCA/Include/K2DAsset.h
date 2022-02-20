#pragma once
#include "KCollider.h"
//KObject -> KCollider -> K2DAsset 상속
class K2DAsset :  public KCollider
{
public:
	float				m_speed;
	float				m_SequenceTimer;
	int					m_AnimIndex;
public:
	RECT				m_rtSource;
	RECT				m_rtDraw;
public:
	virtual void		ObjectOverlap(KCollider* pObj, DWORD dwState);
	virtual void		SelectOverlap(KCollider* pObj, DWORD dwState);
public:
	virtual	void		RectSequence(float speed, int start, int end);
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
		m_SequenceTimer= 0.0f;
		m_AnimIndex = 0;
	};
	~K2DAsset()
	{

	};
};

