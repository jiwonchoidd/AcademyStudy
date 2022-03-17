#pragma once
#include "KCollider.h"
class K3DAsset : public KObject
{
	virtual void		AddPosition(KVector3 vPos);
	virtual void		SetPosition(KVector3 vPos);
	virtual bool		CreateObject(std::wstring vsFile,
		std::wstring psFile, std::wstring tex = nullptr, std::wstring mask = nullptr);
	// 화면좌표 위치를 중점으로 NDC 변환
	virtual void	Convert(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	Convert(std::vector<PNCT_VERTEX>& list,
		std::vector<PNCT_VERTEX>& retList);
	// 화면좌표 위치를 중점으로 NDC 변환
	virtual void	ConvertIndex(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	ConvertIndex(std::vector<PNCT_VERTEX>& list,
		std::vector<PNCT_VERTEX>& retList);
	virtual void		SetUVcoord(std::vector<PNCT_VERTEX>& retList);
public:
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
	virtual bool		CheckVertexData() override;
public:
	K3DAsset();
	~K3DAsset();
};

