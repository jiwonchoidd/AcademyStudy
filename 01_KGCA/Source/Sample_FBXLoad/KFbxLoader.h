#pragma once
#include "KFBXObj.h"
class KFbxLoader
{
public:
	FbxManager*				m_pFbxManager;
	FbxImporter*			m_pFbxImporter;
	FbxScene*				m_pFbxScene; // 내가 원하는 정보를 찾아서 엔진에 맞는 렌더링 데이터로 바꿈
	FbxNode*				m_pRootNode;
	std::vector<KFBXObj*>	m_ObjectList;
public:
	virtual bool			Load(std::wstring filename);
	virtual void			NodeProcess(FbxNode* pParentNode, FbxNode* pNode);
	virtual void			ParseMesh(KFBXObj* pObject);
	virtual std::string		ParseMaterial(FbxSurfaceMaterial* pMtrl);
public:
	virtual void			ReadTextureCoord(FbxMesh* pFbxMesh,
		FbxLayerElementUV* pUVSet,
		int vertexIndex,
		int uvIndex,
		FbxVector2& uv);
	virtual FbxColor		ReadColor(const FbxMesh* mesh,
		DWORD dwVertexColorCount,
		FbxLayerElementVertexColor* pVertexColorSet,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	virtual FbxVector4		ReadNormal(const FbxMesh* mesh,
		int controlPointIndex,
		int vertexCounter);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
};

