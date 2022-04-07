#pragma once
#include "K3DAsset.h"
#include <fbxsdk.h>
struct KBoneWorld
{
	KMatrix  matBoneWorld[255];
};
struct KTrack
{
	UINT	iFrame;
	KMatrix matTrack;
};
class KFBXObj : public K3DAsset
{
public:
	int					m_iIndex = -1;
	KMatrix				m_matLocal;
	KMatrix				m_matAnim;
	std::vector<KTrack>	m_AnimTrack;
public:
	using KSubVertex = std::vector<PNCT_VERTEX>;
	std::vector<KSubVertex>      m_pSubVertexList;
	std::vector<ID3D11Buffer*>   m_pVBList;
public:
	FbxNode*	 m_pFbx_ParentNode;
	FbxNode*	 m_pFbx_ThisNode;
	KFBXObj*	 m_pFbx_ParentObj;
public:
	std::wstring m_tex_name_diffuse;
	std::wstring m_tex_name_normal;
	std::wstring m_tex_name_specular;
	std::vector<KTexture*> m_pTextureList;
public:
	virtual bool		Render(ID3D11DeviceContext* context)override;
	virtual bool		PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)override;
	virtual bool		Release() override;
	virtual bool		CreateVertexData()override;
	virtual HRESULT		CreateVertexBuffer()override;
	virtual bool		CreateIndexData()override; 
public:
	KFBXObj();
	~KFBXObj();
};

