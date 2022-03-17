#pragma once
#include "KCollider.h"
class K3DAsset : public KObject
{
	virtual void		AddPosition(KVector3 vPos);
	virtual void		SetPosition(KVector3 vPos);
	virtual bool		CreateObject(std::wstring vsFile,
		std::wstring psFile, std::wstring tex = nullptr, std::wstring mask = nullptr);
	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	Convert(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	Convert(std::vector<PNCT_VERTEX>& list,
		std::vector<PNCT_VERTEX>& retList);
	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	ConvertIndex(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
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

