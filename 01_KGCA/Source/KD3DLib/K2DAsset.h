#pragma once
#include "KObject.h"
struct Rt_Size
{
	float width;
	float height;
};
class K2DAsset :
    public KObject
{
public:
	KVector2			m_pos;
	RECT				m_rtSource;
	RECT				m_rtDraw;
	Rt_Size				m_rtSize; //사이즈 구조체
	KTexture			m_Masking;
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
};

