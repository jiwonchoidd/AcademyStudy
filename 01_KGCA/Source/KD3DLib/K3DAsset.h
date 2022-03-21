#pragma once
#include "KCollider.h"
class K3DAsset : public KObject
{
public:
	KVector3	m_pos;
	KVector3	m_dir;
public:
	virtual void		AddPosition(KVector3 vPos);
	virtual void		SetPosition(KVector3 vPos);
	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	Convert(
		KVector2 center, float fWidth, float fHeight,
		std::vector<PNCT_VERTEX>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	Convert(std::vector<PNCT_VERTEX>& list,
		std::vector<PNCT_VERTEX>& retList);
	virtual void		SetUVcoord(std::vector<PNCT_VERTEX>& retList);
public:
	virtual bool		CreateVertexData()override;
	virtual bool		CreateIndexData()override;
	virtual bool		CreateTangentSpace(KVector3* v1, KVector3* v2, KVector3* v3, KVector2* uv1,
		KVector2* uv2, KVector2* uv3, KVector3* normal, KVector3* tangent, KVector3* binormal);
public:
	K3DAsset();
	~K3DAsset();
};

