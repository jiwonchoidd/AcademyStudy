#pragma once
#include "KModel.h"
#include "KStd.h"
#include "KTexture.h"
const enum OBJECTCLASSTYPE {
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};
struct KMtrl
{
	FbxNode*			m_pFbxNode;
	FbxSurfaceMaterial* m_pFbxSurfaceMtrl;
	KTexture			m_Texture;
	vector<KMtrl*> m_pSubMtrl;
	KMtrl() {}
	//생성자로 노드와 매터리얼 저장 
	KMtrl(FbxNode* pFbxNode, FbxSurfaceMaterial* pFbxMtrl)
	{
		m_pFbxNode = pFbxNode;
		m_pFbxSurfaceMtrl = pFbxMtrl;
	}
	void Release()
	{
		m_Texture.Release();
		for (auto data : m_pSubMtrl)
		{
			data->Release();
			delete data;
		}
	}
};
//레이어는 하나 일 것이다.
struct KLayer
{
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementUV* pUV;
	FbxLayerElementMaterial* pMaterial;
};

struct KWeight
{
	std::vector<int>     m_IndexList;
	std::vector<float>   m_WegihtList;
};

struct KSkinData
{
	std::vector<FbxNode*>  m_MatrixList;
	//정점당 가중치, 인덱스
	std::vector<KWeight>   m_VertexList;
};
//상속받은 PNCT
struct PNCTIW_VERTEX : public PNCT_VERTEX
{
	int     index[4];
	float   weight[4];
};

struct KMesh : public KModel
{
	//정점당 가중치 인덱스 4개씩 쉐이더에 넣기 위함 
	vector<PNCTIW_VERTEX>	m_WeightList;
	FbxNode*			m_pFbxNode;

	KMesh*				m_pParent; // 부모
	wstring				m_szName;
	wstring				m_szParentName;
	OBJECTCLASSTYPE     m_ClassType;
	vector<KLayer>		m_LayerList;
	int					m_iNumLayer;
	//해당하는 서브매쉬에 서브 매터리얼 연결해야함
	int					m_iMtrlRef;
	KMatrix				m_matWorld;
	vector<KMatrix>		m_AnimationTrack;
	vector<KMesh*>		m_pSubMesh;
	KMesh()
	{
		m_ClassType = CLASS_GEOM;
	}
	bool Release() override
	{
		//부모 해제
		KModel::Release();
		for (auto data : m_pSubMesh)
		{
			data->Release();
			delete data;
		}
		return true;
	}
};
class KFbxObj
{
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
public:
	// todo:게임 오브젝트로 옮겨야함
	bool	m_bAnimPlay = false;
	float   m_fElpaseTime = 0.0f;
	int		m_iAnimIndex = 0;
	float	m_fStartTime;
	float	m_fEndTime;
	float	m_fSampleTime;
	//
	std::vector<FbxNode*>  m_pFbxNodeList;
	std::vector<KMtrl*>  m_pFbxMaterialList;
	std::vector<KMesh*> m_pMeshList;
	CB_DATA				m_cbData;
public:
	KMatrix     DxConvertMatrix(KMatrix m);
	KMatrix     ConvertMatrix(FbxMatrix& m);
	KMatrix		ConvertAMatrix(FbxAMatrix& m);
	bool	LoadObject(std::string filename);
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

