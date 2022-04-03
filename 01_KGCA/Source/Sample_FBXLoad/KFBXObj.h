#pragma once
#include "K3DAsset.h"
#include <fbxsdk.h>
class KFBXObj : public K3DAsset
{
public:
	FbxNode*	 m_pFbx_ParentNode;
	FbxNode*	 m_pFbx_ThisNode;
	KFBXObj*	 m_pFbx_ParentObj;
	std::wstring m_tex_name_diffuse;
	std::wstring m_tex_name_normal;
	std::wstring m_tex_name_specular;
public:
	virtual bool		Render(ID3D11DeviceContext* context);
	virtual bool		CreateVertexData()override;
	virtual bool		CreateIndexData()override; 
public:
	KFBXObj();
	~KFBXObj();
};

