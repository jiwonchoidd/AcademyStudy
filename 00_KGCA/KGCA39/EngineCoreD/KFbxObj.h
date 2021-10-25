#pragma once
#include "KModel.h"
#include "KStd.h"
#include "KTexture.h"
#include "KMesh.h"


class KFbxObj
{
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
public:
	KAnimMatrix     m_matAnimMatrix;
	// todo:게임 오브젝트로 옮겨야함
	bool	m_bAnimPlay = false;
	float   m_fElpaseTime = 0.0f;
	int		m_iAnimIndex = 0;
	float	m_fStartTime;
	float	m_fEndTime;
	float	m_fSampleTime;
	//
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<KMtrl*>		m_pFbxMaterialList;
	std::vector<KMesh*>		m_pMeshList;
	std::vector<KMatrix>	m_matBindPoseList;
	CB_DATA					m_cbData;
public:
	int			GetFindIndex(FbxNode* pNode);
	bool		LoadObject(string filename, string shadername);
	KMatrix     DxConvertMatrix(KMatrix m);
	KMatrix     ConvertMatrix(FbxMatrix& m);
	KMatrix		ConvertAMatrix(FbxAMatrix& m);
	bool	Frame();
	bool    Release();
public:
	void	SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj);
	bool    Render(ID3D11DeviceContext* pContext);
	int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void	LoadMaterial(KMtrl* pMtrl);
	// 일반함수 리스트
public:
	void	PreProcess(FbxNode* pNode);
	void	ParseMesh(FbxNode* pNode, KMesh* pMesh);
	void	ParseNode(FbxNode* pNode, KMesh* pParentMesh);
	KMatrix	ParseTransform(FbxNode* pNode, KMatrix& matParent);
public:
	void	ParseAnimStack(FbxString* szData);
	void	ParseAnimation();
	void	ParseAnimationNode(FbxNode* pNode, KMesh* pMesh);
	bool	ParseMeshSkinning(FbxMesh* pFbxMesh, KMesh* pMesh, KSkinData* pSkindata);
public:
	FbxVector2  ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	FbxVector4  ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
		int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,
		DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);


};

